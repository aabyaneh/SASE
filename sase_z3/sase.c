/*
  This project contains part of the Selfie Project source code
  which is governed by a BSD license. For further information
  and LICENSE conditions see the following website:
  http://selfie.cs.uni-salzburg.at

  Furthermore this project uses the api of z3 SMT solver
  further information: github.com/Z3Prover/z3
*/

#include "sase.h"

// -----------------------------------------------------------------
// ---------------- Solver Aided Symbolic Execution ----------------
// -----------------------------------------------------------------

char      var_buffer[100];   // a buffer for automatic variable name generation
context   ctx;
solver    slv(ctx);
expr      zero_bv(ctx);
expr      one_bv(ctx);
expr      eight_bv(ctx);
expr      meight_bv(ctx);

uint64_t  sase_symbolic = 0; // flag for symbolically executing code
uint64_t  b             = 0; // counting total number of backtracking
uint64_t  SASE          = 8; // Solver Aided Symbolic Execution
uint8_t   CONCRETE_T    = 0; // concrete value type
uint8_t   SYMBOLIC_T    = 1; // symbolic value type

// symbolic registers
expr*     sase_regs;         // array of pointers to SMT expressions
uint8_t*  sase_regs_typ;     // CONCRETE_T or SYMBOLIC_T

// engine trace
uint64_t  sase_trace_size = 10000000;
uint64_t  sase_tc         = 0;    // trace counter
uint64_t* sase_pcs;
expr*     sase_false_branchs;
uint64_t* sase_read_trace_ptrs;   // pointers to read trace
uint64_t* sase_program_brks;      // keep track of program_break
uint64_t* sase_store_trace_ptrs;  // pointers to store trace
uint64_t* sase_rds;
uint64_t  mrif          = 0;      // most recent conditional expression
uint8_t   which_branch  = 0;      // which branch is taken
uint8_t   assert_zone   = 0;      // is assertion zone?

// store trace
uint64_t  tc            = 0;
uint64_t* tcs;
uint64_t* vaddrs;
uint64_t* values;
uint8_t*  is_symbolics;
expr*     symbolic_values;

// read trace
uint64_t* concrete_reads;
expr*     constrained_reads;
uint64_t  read_tc         = 0;
uint64_t  read_tc_current = 0;
uint64_t  read_buffer     = 0;

// input trace
expr*     constrained_inputs;
uint64_t* sase_input_trace_ptrs;
uint64_t  input_cnt         = 0;
uint64_t  input_cnt_current = 0;

// ********************** engine functions ************************

void init_sase() {
  zero_bv   = ctx.bv_val(0, 64);
  one_bv    = ctx.bv_val(1, 64);
  eight_bv  = ctx.bv_val(8, 64);
  meight_bv = ctx.bv_val(-8u, 64);

  sase_regs              = (expr*)    malloc(sizeof(expr)    * NUMBEROFREGISTERS);
  sase_regs_typ          = (uint8_t*) malloc(sizeof(uint8_t) * NUMBEROFREGISTERS);
  for (size_t i = 0; i < NUMBEROFREGISTERS; i++) {
    sase_regs_typ[i] = CONCRETE_T;
  }
  sase_regs[REG_ZR] = zero_bv;
  sase_regs[REG_FP] = zero_bv;

  sase_pcs              = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  sase_false_branchs    = (expr*)     malloc(sizeof(expr)     * sase_trace_size);
  sase_read_trace_ptrs  = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  sase_program_brks     = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  sase_store_trace_ptrs = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  sase_rds              = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);

  tcs                   = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  vaddrs                = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  values                = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  is_symbolics          = (uint8_t*)  malloc(sizeof(uint8_t)  * sase_trace_size);
  symbolic_values       = (expr*)     malloc(sizeof(expr)     * sase_trace_size);

  concrete_reads        = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);
  constrained_reads     = (expr*)     malloc(sizeof(expr)     * sase_trace_size);

  constrained_inputs    = (expr*)     malloc(sizeof(expr)     * sase_trace_size);
  sase_input_trace_ptrs = (uint64_t*) malloc(sizeof(uint64_t) * sase_trace_size);

  // initialization
  *tcs             = 0;
  *vaddrs          = 0;
  *is_symbolics    = CONCRETE_T;
  *symbolic_values = ctx.bv_val(*values, 64);
}

uint64_t is_trace_space_available() {
  return tc + 1 < sase_trace_size;
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
    sase_regs[rd] = ctx.bv_val(imm << 12, 64);

    sase_regs_typ[rd] = CONCRETE_T;
  }
}

void sase_addi() {
  if (rd != REG_ZR) {
    if (imm == 8) {
      sase_regs[rd] = sase_regs[rs1] + eight_bv;
    } else if (imm == 0) {
      sase_regs[rd] = sase_regs[rs1] + zero_bv;
    } else if (imm == -8u) {
      sase_regs[rd] = sase_regs[rs1] + meight_bv;
    } else if (imm == 1) {
      sase_regs[rd] = sase_regs[rs1] + one_bv;
    } else
      sase_regs[rd] = sase_regs[rs1] + ctx.bv_val(imm, 64);

    sase_regs_typ[rd] = sase_regs_typ[rs1];
  }
}

void sase_add() {
  if (rd != REG_ZR) {
    sase_regs[rd] = sase_regs[rs1] + sase_regs[rs2];

    sase_regs_typ[rd] = sase_regs_typ[rs1] | sase_regs_typ[rs2];
  }
}

void sase_sub() {
  if (rd != REG_ZR) {
    sase_regs[rd] = sase_regs[rs1] - sase_regs[rs2];

    sase_regs_typ[rd] = sase_regs_typ[rs1] | sase_regs_typ[rs2];
  }
}

void sase_mul() {
  if (rd != REG_ZR) {
    sase_regs[rd] = sase_regs[rs1] * sase_regs[rs2];

    sase_regs_typ[rd] = sase_regs_typ[rs1] | sase_regs_typ[rs2];
  }
}

void sase_divu() {
  // check if divisor is zero?
  slv.push();
  slv.add(sase_regs[rs2] == zero_bv);
  if (slv.check() == sat) {
    printf("OUTPUT: SE division by zero! at pc %llx \n", pc - entry_point);
    printf("backtracking: %llu \n", b);
    std::cout << slv.get_model() << "\n";
    exit((int) EXITCODE_SYMBOLICEXECUTIONERROR);
  }
  slv.pop();

  // divu semantics
  if (rd != REG_ZR) {
    sase_regs[rd] = udiv(sase_regs[rs1], sase_regs[rs2]);

    sase_regs_typ[rd] = sase_regs_typ[rs1] | sase_regs_typ[rs2];
  }
}

void sase_remu() {
  // check if divisor is zero?
  slv.push();
  slv.add(sase_regs[rs2] == zero_bv);
  if (slv.check() == sat) {
    printf("OUTPUT: SE division by zero! at pc %llx \n", pc - entry_point);
    printf("backtracking: %llu \n", b);
    std::cout << slv.get_model() << "\n";
    exit((int) EXITCODE_SYMBOLICEXECUTIONERROR);
  }
  slv.pop();

  // remu semantics
  if (rd != REG_ZR) {
    sase_regs[rd] = urem(sase_regs[rs1], sase_regs[rs2]);

    sase_regs_typ[rd] = sase_regs_typ[rs1] | sase_regs_typ[rs2];
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
        sase_false_branchs[sase_tc]    = ult(sase_regs[rs1], sase_regs[rs2]);
        sase_pcs[sase_tc]              = pc  + 3 * INSTRUCTIONSIZE;

        slv.push();
        slv.add(uge(sase_regs[rs1], sase_regs[rs2]));

        // skip execution of next two instructions
        pc = pc + 3 * INSTRUCTIONSIZE;
        ic_addi = ic_addi + 1;
        ic_sub  = ic_sub  + 1;
      }
    }

    if (is_branch == 1) {
      sase_false_branchs[sase_tc]    = uge(sase_regs[rs1], sase_regs[rs2]);
      sase_pcs[sase_tc]              = pc  + INSTRUCTIONSIZE;

      slv.push();
      slv.add(ult(sase_regs[rs1], sase_regs[rs2]));

      pc = pc + INSTRUCTIONSIZE;
    }

    if (assert_zone == 0) {
      // symbolic semantics
      sase_program_brks[sase_tc]     = get_program_break(current_context);
      sase_read_trace_ptrs[sase_tc]  = read_tc_current;
      sase_input_trace_ptrs[sase_tc] = input_cnt_current;
      sase_store_trace_ptrs[sase_tc] = mrif;
      mrif = tc;
      store_registers_fp_sp_rd(); // after mrif =
      sase_tc++;

      if (slv.check() == sat) {
        sase_regs[rd]     = one_bv;
        sase_regs_typ[rd] = CONCRETE_T;
        *(registers + rd) = 1;
      } else {
        // printf("%s\n", "unreachable branch true!");
        sase_backtrack_sltu(1);
      }
    } else {
      slv.pop();
    }

  } else
    pc = pc + INSTRUCTIONSIZE;
}

void sase_backtrack_sltu(int is_true_branch_unreachable) {
  if (sase_tc == 0) {
    // printf("pc: %llx, read_tc: %llu, arg: %d\n", pc - entry_point, read_tc, is_true_branch_unreachable);
    pc = 0;
    return;
  }

  sase_tc--;
  pc                = sase_pcs[sase_tc];
  read_tc_current   = sase_read_trace_ptrs[sase_tc];
  input_cnt_current = sase_input_trace_ptrs[sase_tc];
  set_program_break(current_context, sase_program_brks[sase_tc]);
  backtrack_branch_stores(); // before mrif =
  mrif = sase_store_trace_ptrs[sase_tc];

  slv.pop();
  slv.add(sase_false_branchs[sase_tc]);
  if (slv.check() == unsat) {
    if (is_true_branch_unreachable) {
      printf("%s\n", "unreachable branch both true and false!");
      exit((int) EXITCODE_SYMBOLICEXECUTIONERROR);
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

        // if (mrv == 0)
        //   printf("OUTPUT: uninitialize memory address %llu at pc %x\n", vaddr, pc - entry_point);

        sase_regs_typ[rd] = *(is_symbolics    + mrv);
        sase_regs[rd]     = *(symbolic_values + mrv);
        registers[rd]     = *(values          + mrv);

        pc = pc + INSTRUCTIONSIZE;
        ic_ld = ic_ld + 1;

      } else
        pc = pc + INSTRUCTIONSIZE;
    } else
      throw_exception(EXCEPTION_PAGEFAULT, get_page_of_virtual_address(vaddr));
  } else
    throw_exception(EXCEPTION_INVALIDADDRESS, vaddr);
}

void sase_sd() {
  uint64_t vaddr = *(registers + rs1) + imm;

  if (is_valid_virtual_address(vaddr)) {
    if (is_virtual_address_mapped(pt, vaddr)) {

      sase_store_memory(pt, vaddr, sase_regs_typ[rs2], registers[rs2], sase_regs[rs2]);

      pc = pc + INSTRUCTIONSIZE;
      ic_sd = ic_sd + 1;
    } else
      throw_exception(EXCEPTION_PAGEFAULT, get_page_of_virtual_address(vaddr));
  } else
    throw_exception(EXCEPTION_INVALIDADDRESS, vaddr);
}

void sase_jal_jalr() {
  if (rd != REG_ZR) {
    sase_regs[rd] = ctx.bv_val(registers[rd], 64);

    sase_regs_typ[rd] = CONCRETE_T;
  }
}

void sase_store_memory(uint64_t* pt, uint64_t vaddr, uint8_t is_symbolic, uint64_t value, expr& sym_value) {
  uint64_t mrv;

  mrv = load_symbolic_memory(pt, vaddr);

  if (mrv != 0)
    if (is_symbolic == *(is_symbolics + mrv))
      if (value == *(values + mrv))
        if (eq(sym_value, *(symbolic_values + mrv)))
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
  while (mrif < tc) {
    if (*(vaddrs + tc) < NUMBEROFREGISTERS) {
      restore_registers_fp_sp_rd(tc, *(vaddrs + tc));
    } else {
      store_virtual_memory(pt, *(vaddrs + tc), *(tcs + tc));
    }
    tc--;
  }
}