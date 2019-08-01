/*
  This project contains part of the Selfie Project source code
  which is governed by a BSD license. For further information
  and LICENSE conditions see the following website:
  http://selfie.cs.uni-salzburg.at

  Furthermore this project uses the api of boolector SMT solver
  further information: boolector.github.io
*/

#include "sase.h"

// -----------------------------------------------------------------
// ---------------- Solver Aided Symbolic Execution ----------------
// -----------------------------------------------------------------

char           var_buffer[100];   // a buffer for automatic variable name generation
Btor*          btor;
BoolectorSort  bv_sort;
BoolectorSort  bv_sort_32;
BoolectorNode* zero_bv;
BoolectorNode* one_bv;
BoolectorNode* eight_bv;
BoolectorNode* meight_bv;
BoolectorNode* twelve_bv;

uint64_t       sase_symbolic = 0; // flag for symbolically executing code
uint64_t       b             = 0; // counting total number of backtracking
uint64_t       SASE          = 8; // Solver Aided Symbolic Execution
uint64_t       CONCRETE_T    = 0; // concrete value type
uint64_t       SYMBOLIC_T    = 1; // symbolic value type
uint64_t       two_to_the_power_of_32;

// symbolic registers
BoolectorNode**   sase_regs;         // array of pointers to SMT expressions
uint64_t*         sase_regs_typ;     // CONCRETE_T or SYMBOLIC_T

// engine trace
uint64_t        sase_trace_size = 10000000;
uint64_t        sase_tc = 0;            // trace counter
uint64_t*       sase_pcs;
BoolectorNode** sase_false_branchs;
uint64_t*       sase_read_trace_ptrs;   // pointers to read trace
uint64_t*       sase_program_brks;      // keep track of program_break
uint64_t*       sase_store_trace_ptrs;  // pointers to store trace
uint64_t*       sase_rds;
uint64_t        mrif          = 0;      // most recent conditional expression
uint8_t         which_branch  = 0;      // which branch is taken
uint8_t         assert_zone   = 0;      // is assertion zone?

// store trace
uint64_t        tc;
uint64_t*       tcs;
uint64_t*       vaddrs;
uint64_t*       values;
uint8_t*        is_symbolics;
BoolectorNode** symbolic_values;

// read trace
uint64_t*       concrete_reads;
BoolectorNode** constrained_reads;
uint64_t        read_tc         = 0;
uint64_t        read_tc_current = 0;
uint64_t        read_buffer     = 0;

uint64_t MASK_LO = 4294967295;
uint64_t MASK_HI = 18446744069414584320;

// ********************** engine functions ************************

void init_sase() {
  uint64_t t  = -8;

  btor        = boolector_new();
  bv_sort     = boolector_bitvec_sort(btor, 64);
  bv_sort_32  = boolector_bitvec_sort(btor, 32);
  zero_bv     = boolector_unsigned_int(btor, 0, bv_sort);
  one_bv      = boolector_unsigned_int(btor, 1, bv_sort);
  twelve_bv   = boolector_unsigned_int(btor, 12, bv_sort);
  eight_bv    = boolector_unsigned_int(btor, 8, bv_sort);
  meight_bv   = boolector_unsigned_int_64(t);

  boolector_set_opt(btor, BTOR_OPT_INCREMENTAL, 1);
  boolector_set_opt(btor, BTOR_OPT_MODEL_GEN, 1);

  two_to_the_power_of_32 = two_to_the_power_of(32);

  sase_regs     = malloc(sizeof(BoolectorNode*) * NUMBEROFREGISTERS);
  sase_regs_typ = malloc(sizeof(uint64_t)       * NUMBEROFREGISTERS);
  for (size_t i = 0; i < NUMBEROFREGISTERS; i++) {
    sase_regs_typ[i] = CONCRETE_T;
  }
  sase_regs[REG_ZR] = zero_bv;
  sase_regs[REG_FP] = zero_bv;

  sase_pcs              = malloc(sizeof(uint64_t)       * sase_trace_size);
  sase_false_branchs    = malloc(sizeof(BoolectorNode*) * sase_trace_size);
  sase_read_trace_ptrs  = malloc(sizeof(uint64_t)       * sase_trace_size);
  sase_program_brks     = malloc(sizeof(uint64_t)       * sase_trace_size);
  sase_store_trace_ptrs = malloc(sizeof(uint64_t)       * sase_trace_size);
  sase_rds              = malloc(sizeof(uint64_t)       * sase_trace_size);

  tcs                   = malloc(sizeof(uint64_t)       * sase_trace_size);
  vaddrs                = malloc(sizeof(uint64_t)       * sase_trace_size);
  values                = malloc(sizeof(uint64_t)       * sase_trace_size);
  is_symbolics          = malloc(sizeof(uint8_t)        * sase_trace_size);
  symbolic_values       = malloc(sizeof(BoolectorNode*) * sase_trace_size);

  concrete_reads        = malloc(sizeof(uint64_t)       * sase_trace_size);
  constrained_reads     = malloc(sizeof(BoolectorNode*) * sase_trace_size);
}

uint64_t is_trace_space_available() {
  return tc + 1 < sase_trace_size;
}

BoolectorNode* boolector_unsigned_int_64(uint64_t value) {
  return boolector_concat(btor, boolector_unsigned_int(btor, value & MASK_HI, bv_sort_32), boolector_unsigned_int(btor, value & MASK_LO, bv_sort_32));
}

void store_registers_fp_sp_rd() {
  if (tc + 2 >= sase_trace_size)
    throw_exception(EXCEPTION_MAXTRACE, 0);

  tc++;
  *(tcs             + tc) = 0;
  *(is_symbolics    + tc) = 0;
  *(values          + tc) = *(registers + REG_FP);
  *(symbolic_values + tc) = sase_regs[REG_FP];
  *(vaddrs + tc)          = rd;

  tc++;
  *(tcs             + tc) = 0;
  *(is_symbolics    + tc) = 0;
  *(values          + tc) = *(registers + REG_SP);
  *(symbolic_values + tc) = sase_regs[REG_SP];
  *(vaddrs + tc)          = rd;
}

void restore_registers_fp_sp_rd(uint64_t tr_cnt, uint64_t rd_reg) {
  registers[REG_SP]     = *(values + tr_cnt);
  sase_regs[REG_SP]     = *(symbolic_values + tr_cnt);
  sase_regs_typ[REG_SP] = CONCRETE_T;
  tr_cnt--;
  tc--;
  registers[REG_FP]     = *(values + tr_cnt);
  sase_regs[REG_FP]     = *(symbolic_values + tr_cnt);
  sase_regs_typ[REG_FP] = CONCRETE_T;

  registers[rd_reg] = 0;
  sase_regs[rd_reg] = zero_bv;
  sase_regs_typ[rd_reg] = CONCRETE_T;
}

uint8_t check_next_1_instrs() {
  uint64_t op;

  pc = pc + INSTRUCTIONSIZE;
  fetch();
  pc = pc - INSTRUCTIONSIZE;
  op = get_opcode(ir);
  if (op == OP_BRANCH)
    return 1;
  else
    return 0;
}

uint8_t match_sub(uint64_t prev_instr_rd) {
  uint64_t rs1_;
  uint64_t rs2_;
  uint64_t rd_;
  uint64_t funct3_;
  uint64_t funct7_;

  funct7_ = get_funct7(ir);
  funct3_ = get_funct3(ir);
  rs1_    = get_rs1(ir);
  rs2_    = get_rs2(ir);
  rd_     = get_rd(ir);

  if (funct3_ == F3_ADD) {
    if (funct7_ == F7_SUB)
      if (rs1_ == prev_instr_rd)
        if (rs2_ == rd)
          if (rd_ == rs2_)
            return 1;
  }

  return 0;
}

uint8_t match_addi() {
  uint64_t rs1_;
  uint64_t rd_;
  uint64_t funct3_;
  uint64_t imm_;

  rs1_    = get_rs1(ir);
  rd_     = get_rd(ir);
  funct3_ = get_funct3(ir);
  imm_    = get_immediate_i_format(ir);

  if (funct3_ == F3_ADDI) {
    if (imm_ == 1)
      if (rs1_ == REG_ZR)
        if (rd_ != rd)
          return 1;
  }

  return 0;
}

uint8_t check_next_3_instrs() {
  uint64_t rd_;
  uint64_t opcode_;
  uint64_t saved_pc;

  saved_pc = pc;

  pc = saved_pc + INSTRUCTIONSIZE;
  fetch();
  opcode_ = get_opcode(ir);
  if (opcode_ == OP_IMM) {
    if (match_addi()) {
      rd_ = get_rd(ir);
      pc = saved_pc + 2 * INSTRUCTIONSIZE;
      fetch();
      opcode_ = get_opcode(ir);
      if (opcode_ == OP_OP) {
        if (match_sub(rd_)) {
          rd = get_rd(ir);
          pc = saved_pc;
          return 2;
        }
      }
    }
  }

  pc = saved_pc;
  return 1;
}

// ********************** engine instructions ************************

void sase_lui() {
  if (rd != REG_ZR) {
    if (imm < two_to_the_power_of_32)
      sase_regs[rd] = boolector_sll(btor, boolector_unsigned_int(btor, imm, bv_sort), twelve_bv);
    else
      sase_regs[rd] = boolector_unsigned_int_64(imm << 12);

    sase_regs_typ[rd] = CONCRETE_T;
  }
}

void sase_addi() {
  if (rd != REG_ZR) {
    if (imm == 8) {
      sase_regs[rd] = boolector_add(btor, sase_regs[rs1], eight_bv);
    } else if (imm == 0) {
      sase_regs[rd] = boolector_add(btor, sase_regs[rs1], zero_bv);
    } else if (imm == -8) {
      sase_regs[rd] = boolector_add(btor, sase_regs[rs1], meight_bv);
    } else if (imm == 1) {
      sase_regs[rd] = boolector_add(btor, sase_regs[rs1], one_bv);
    } else {
      if (imm < two_to_the_power_of_32) {
        sase_regs[rd] = boolector_add(btor, sase_regs[rs1], boolector_unsigned_int(btor, imm, bv_sort));
      } else {
        sase_regs[rd] = boolector_add(btor, sase_regs[rs1], boolector_unsigned_int_64(imm));
      }
    }

    sase_regs_typ[rd] = sase_regs_typ[rs1];
  }
}

void sase_add() {
  if (rd != REG_ZR) {
    sase_regs[rd] = boolector_add(btor, sase_regs[rs1], sase_regs[rs2]);

    if (sase_regs_typ[rs1] == CONCRETE_T && sase_regs_typ[rs2] == CONCRETE_T)
      sase_regs_typ[rd] = CONCRETE_T;
    else
      sase_regs_typ[rd] = SYMBOLIC_T;
  }
}

void sase_sub() {
  if (rd != REG_ZR) {
    sase_regs[rd] = boolector_sub(btor, sase_regs[rs1], sase_regs[rs2]);

    if (sase_regs_typ[rs1] == CONCRETE_T && sase_regs_typ[rs2] == CONCRETE_T)
      sase_regs_typ[rd] = CONCRETE_T;
    else
      sase_regs_typ[rd] = SYMBOLIC_T;
  }
}

void sase_mul() {
  if (rd != REG_ZR) {
    sase_regs[rd] = boolector_mul(btor, sase_regs[rs1], sase_regs[rs2]);

    if (sase_regs_typ[rs1] == CONCRETE_T && sase_regs_typ[rs2] == CONCRETE_T)
      sase_regs_typ[rd] = CONCRETE_T;
    else
      sase_regs_typ[rd] = SYMBOLIC_T;
  }
}

void sase_divu() {
  // check if divisor is zero?
  boolector_push(btor, 1);
  boolector_assert(btor, boolector_eq(btor, sase_regs[rs2], zero_bv));
  if (boolector_sat(btor) == BOOLECTOR_SAT) {
    printf("%s\n", "SE division by zero!");
    printf("pc: %llx\n", pc - entry_point);
    boolector_print_model (btor, "smt2", stdout);
    // exit(EXITCODE_SYMBOLICEXECUTIONERROR);
  }
  boolector_pop(btor, 1);

  // divu semantics
  if (rd != REG_ZR) {
    sase_regs[rd] = boolector_udiv(btor, sase_regs[rs1], sase_regs[rs2]);

    if (sase_regs_typ[rs1] == CONCRETE_T && sase_regs_typ[rs2] == CONCRETE_T)
      sase_regs_typ[rd] = CONCRETE_T;
    else
      sase_regs_typ[rd] = SYMBOLIC_T;
  }
}

void sase_remu() {
  // check if divisor is zero?
  boolector_push(btor, 1);
  boolector_assert(btor, boolector_eq(btor, sase_regs[rs2], zero_bv));
  if (boolector_sat (btor) == BOOLECTOR_SAT) {
    printf("%s\n", "SE division by zero!");
    printf("pc: %llx\n", pc - entry_point);
    boolector_print_model (btor, "smt2", stdout);
    // exit(EXITCODE_SYMBOLICEXECUTIONERROR);
  }
  boolector_pop(btor, 1);

  // remu semantics
  if (rd != REG_ZR) {
    sase_regs[rd] = boolector_urem(btor, sase_regs[rs1], sase_regs[rs2]);

    if (sase_regs_typ[rs1] == CONCRETE_T && sase_regs_typ[rs2] == CONCRETE_T)
      sase_regs_typ[rd] = CONCRETE_T;
    else
      sase_regs_typ[rd] = SYMBOLIC_T;
  }
}

void sase_sltu() {
  uint8_t  is_branch;
  uint64_t op;
  uint64_t saved_pc;

  ic_sltu = ic_sltu + 1;

  if (rd != REG_ZR) {
    which_branch = 0;

    // concrete semantics
    if (sase_regs_typ[rs1] == CONCRETE_T && sase_regs_typ[rs2] == CONCRETE_T) {
      if (*(registers + rs1) < *(registers + rs2)) {
        *(registers + rd) = 1;
        sase_regs[rd]     = one_bv;
      } else {
        *(registers + rd) = 0;
        sase_regs[rd]     = zero_bv;
      }

      which_branch = 1;

      sase_regs_typ[rd] = CONCRETE_T;
      pc = pc + INSTRUCTIONSIZE;
      return;
    }

    is_branch = check_next_1_instrs();
    if (is_branch == 0) {
      is_branch = check_next_3_instrs();

      if (is_branch == 2) {
        sase_false_branchs[sase_tc]    = boolector_ult(btor, sase_regs[rs1], sase_regs[rs2]);
        sase_pcs[sase_tc]              = pc  + 3 * INSTRUCTIONSIZE;

        boolector_push(btor, 1);
        boolector_assert(btor, boolector_ugte(btor, sase_regs[rs1], sase_regs[rs2]));

        // skip execution of next two instructions
        pc = pc + 3 * INSTRUCTIONSIZE;
        ic_addi = ic_addi + 1;
        ic_sub  = ic_sub  + 1;
      }
    }

    if (is_branch == 1) {
      sase_false_branchs[sase_tc]    = boolector_ugte(btor, sase_regs[rs1], sase_regs[rs2]);
      sase_pcs[sase_tc]              = pc  + INSTRUCTIONSIZE;

      boolector_push(btor, 1);
      boolector_assert(btor, boolector_ult(btor, sase_regs[rs1], sase_regs[rs2]));
      pc = pc + INSTRUCTIONSIZE;
    }

    if (assert_zone == 0) {
      // symbolic semantics
      sase_program_brks[sase_tc]     = get_program_break(current_context);
      sase_read_trace_ptrs[sase_tc]  = read_tc_current;
      sase_store_trace_ptrs[sase_tc] = mrif;
      mrif = tc;
      store_registers_fp_sp_rd(); // after mrif =
      sase_tc++;

      if (boolector_sat(btor) == BOOLECTOR_SAT) {
        sase_regs[rd]     = one_bv;
        sase_regs_typ[rd] = CONCRETE_T;
        *(registers + rd) = 1;
      } else {
        // printf("%s\n", "unreachable branch true!");
        sase_backtrack_sltu(1);
      }
    } else {
      boolector_pop(btor, 1);
    }

  }
}

void sase_backtrack_sltu(int is_true_branch_unreachable) {
  if (sase_tc == 0) {
    // printf("pc: %llx, read_tc: %llu, arg: %d\n", pc - entry_point, read_tc, is_true_branch_unreachable);
    pc = 0;
    return;
  }

  sase_tc--;
  pc              = sase_pcs[sase_tc];
  read_tc_current = sase_read_trace_ptrs[sase_tc];
  set_program_break(current_context, sase_program_brks[sase_tc]);
  backtrack_branch_stores(); // before mrif =
  mrif = sase_store_trace_ptrs[sase_tc];

  boolector_pop(btor, 1);
  boolector_assert(btor, sase_false_branchs[sase_tc]);
  if (boolector_sat(btor) != BOOLECTOR_SAT) {
    if (is_true_branch_unreachable) {
      printf("%s\n", "unreachable branch both true and false!");
      // exit(EXITCODE_SYMBOLICEXECUTIONERROR);
    } else {
      // printf("%s %llu\n", "unreachable branch false!", pc);
      sase_backtrack_sltu(0);
    }
  } else {
    sase_regs[rd]     = zero_bv;
    sase_regs_typ[rd] = CONCRETE_T;
    *(registers + rd) = 0;
  }
}

void sase_ld() {
  uint64_t mrv;
  uint64_t vaddr = *(registers + rs1) + imm;

  if (is_valid_virtual_address(vaddr)) {
    if (is_virtual_address_mapped(pt, vaddr)) {
      if (rd != REG_ZR) {
        mrv = load_symbolic_memory(pt, vaddr);

        // if (mrv == 0) {
        //   printf("%s %llu %x\n", "uninitialize memory", vaddr, pc - entry_point);
        // }

        if (*(is_symbolics + mrv) == CONCRETE_T) {
          sase_regs_typ[rd] = CONCRETE_T;
          if (!mrv) { //*(symbolic_values + mrv) == 0) {
            if (*(values + mrv) < two_to_the_power_of_32)
              sase_regs[rd]     = boolector_unsigned_int(btor, *(values + mrv), bv_sort);
            else {
              // printf("large load pc: %llx mrv: %llu val: %llu\n", pc-entry_point, mrv, *(values + mrv));
              sase_regs[rd]     = boolector_unsigned_int_64(*(values + mrv));
            }
          } else {
            sase_regs[rd]     = *(symbolic_values + mrv);
          }
        } else {
          sase_regs_typ[rd] = SYMBOLIC_T;
          sase_regs[rd]     = *(symbolic_values + mrv);
        }

        *(registers + rd) = *(values + mrv);

        pc = pc + INSTRUCTIONSIZE;
        ic_ld = ic_ld + 1;
      }
    } else
      throw_exception(EXCEPTION_PAGEFAULT, get_page_of_virtual_address(vaddr));
  } else
    throw_exception(EXCEPTION_INVALIDADDRESS, vaddr);
}

void sase_sd() {
  uint64_t vaddr = *(registers + rs1) + imm;

  if (is_valid_virtual_address(vaddr)) {
    if (is_virtual_address_mapped(pt, vaddr)) {

      if (sase_regs_typ[rs2] == CONCRETE_T) {
        sase_store_memory(pt, vaddr, CONCRETE_T, *(registers + rs2), sase_regs[rs2]);
      } else {
        sase_store_memory(pt, vaddr, SYMBOLIC_T, *(registers + rs2), sase_regs[rs2]);
      }

      pc = pc + INSTRUCTIONSIZE;
      ic_sd = ic_sd + 1;
    } else
      throw_exception(EXCEPTION_PAGEFAULT, get_page_of_virtual_address(vaddr));
  } else
    throw_exception(EXCEPTION_INVALIDADDRESS, vaddr);
}

void sase_jal_jalr() {
  if (rd != REG_ZR) {
    // assert: *(registers + rd) < 2^32
    sase_regs[rd] = boolector_unsigned_int(btor, *(registers + rd), bv_sort);

    sase_regs_typ[rd] = CONCRETE_T;
  }
}

void sase_store_memory(uint64_t* pt, uint64_t vaddr, uint8_t is_symbolic, uint64_t value, BoolectorNode* sym_value) {
  uint64_t mrv;

  mrv = load_symbolic_memory(pt, vaddr);

  if (mrv != 0)
    if (is_symbolic == *(is_symbolics + mrv))
      if (value == *(values + mrv))
        if (sym_value == *(symbolic_values + mrv))
          return;

  if (mrif < mrv && vaddr != read_buffer) {
    *(is_symbolics    + mrv) = is_symbolic;
    *(values          + mrv) = value;
    *(symbolic_values + mrv) = sym_value;
  } else if (is_trace_space_available()) {
    tc++;

    *(tcs             + tc) = mrv;
    *(is_symbolics    + tc) = is_symbolic;
    *(values          + tc) = value;
    *(symbolic_values + tc) = sym_value;
    *(vaddrs          + tc) = vaddr;

    store_virtual_memory(pt, vaddr, tc);
  } else
    throw_exception(EXCEPTION_MAXTRACE, 0);
}

void backtrack_branch_stores() {
  uint64_t pre_tc;

  while (mrif < tc) {
    if (*(vaddrs + tc) < NUMBEROFREGISTERS) {
      restore_registers_fp_sp_rd(tc, *(vaddrs + tc));
    } else {
      store_virtual_memory(pt, *(vaddrs + tc), *(tcs + tc));
    }
    tc--;
  }
}

/* -----------------------------------------------------------------
------------------- Modified functions in Selfie.c:-----------------

void implement_read(uint64_t* context);
void implement_write(uint64_t* context);
void implement_open(uint64_t* context);
void implement_brk(uint64_t* context);
void decode_execute();
uint64_t down_load_string(uint64_t* table, uint64_t vaddr, uint64_t* s);
void map_and_store(uint64_t* context, uint64_t vaddr, uint64_t data);
void up_load_arguments(uint64_t* context, uint64_t argc, uint64_t* argv);
uint64_t selfie_run(uint64_t machine);

------------------------------------------------------------------- */