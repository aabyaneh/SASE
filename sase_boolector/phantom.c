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
// ----------------------- LIBRARY PROCEDURES ----------------------
// -----------------------------------------------------------------

void init_library();
void reset_library();

uint64_t two_to_the_power_of(uint64_t p);
uint64_t ten_to_the_power_of(uint64_t p);

uint64_t left_shift(uint64_t n, uint64_t b);
uint64_t right_shift(uint64_t n, uint64_t b);

uint64_t get_bits(uint64_t n, uint64_t i, uint64_t b);
uint64_t get_low_word(uint64_t n);
uint64_t get_high_word(uint64_t n);

uint64_t abs(uint64_t n);

uint64_t signed_less_than(uint64_t a, uint64_t b);
uint64_t signed_division(uint64_t a, uint64_t b);

uint64_t is_signed_integer(uint64_t n, uint64_t b);
uint64_t sign_extend(uint64_t n, uint64_t b);
uint64_t sign_shrink(uint64_t n, uint64_t b);

uint64_t  load_character(uint64_t* s, uint64_t i);

uint64_t  string_length(uint64_t* s);
void      string_reverse(uint64_t* s);
uint64_t  string_compare(uint64_t* s, uint64_t* t);

uint64_t  atoi(uint64_t* s);
uint64_t* itoa(uint64_t n, uint64_t* s, uint64_t b, uint64_t a);

uint64_t fixed_point_ratio(uint64_t a, uint64_t b, uint64_t f);
uint64_t fixed_point_percentage(uint64_t r, uint64_t f);

void put_character(uint64_t c);

void print(uint64_t* s);
void println();

void print_character(uint64_t c);
void print_string(uint64_t* s);
void print_integer(uint64_t n);
void unprint_integer(uint64_t n);
void print_hexadecimal(uint64_t n, uint64_t a);
void print_octal(uint64_t n, uint64_t a);
void print_binary(uint64_t n, uint64_t a);

uint64_t print_format0(uint64_t* s, uint64_t i);
uint64_t print_format1(uint64_t* s, uint64_t i, uint64_t* a);

void printf1(uint64_t* s, uint64_t* a1);
void printf2(uint64_t* s, uint64_t* a1, uint64_t* a2);
void printf3(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3);
void printf4(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3, uint64_t* a4);
void printf5(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3, uint64_t* a4, uint64_t* a5);
void printf6(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3, uint64_t* a4, uint64_t* a5, uint64_t* a6);

uint64_t round_up(uint64_t n, uint64_t m);

uint64_t* smalloc(uint64_t size);
uint64_t* zalloc(uint64_t size);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t CHAR_EOF          =  -1; // end of file
uint64_t CHAR_BACKSPACE    =   8; // ASCII code 8  = backspace
uint64_t CHAR_TAB          =   9; // ASCII code 9  = tabulator
uint64_t CHAR_LF           =  10; // ASCII code 10 = line feed
uint64_t CHAR_CR           =  13; // ASCII code 13 = carriage return
uint64_t CHAR_SPACE        = ' ';
uint64_t CHAR_SEMICOLON    = ';';
uint64_t CHAR_PLUS         = '+';
uint64_t CHAR_DASH         = '-';
uint64_t CHAR_ASTERISK     = '*';
uint64_t CHAR_SLASH        = '/';
uint64_t CHAR_UNDERSCORE   = '_';
uint64_t CHAR_EQUAL        = '=';
uint64_t CHAR_LPARENTHESIS = '(';
uint64_t CHAR_RPARENTHESIS = ')';
uint64_t CHAR_LBRACE       = '{';
uint64_t CHAR_RBRACE       = '}';
uint64_t CHAR_COMMA        = ',';
uint64_t CHAR_LT           = '<';
uint64_t CHAR_GT           = '>';
uint64_t CHAR_EXCLAMATION  = '!';
uint64_t CHAR_PERCENTAGE   = '%';
uint64_t CHAR_SINGLEQUOTE  =  39; // ASCII code 39 = '
uint64_t CHAR_DOUBLEQUOTE  = '"';
uint64_t CHAR_BACKSLASH    =  92; // ASCII code 92 = backslash

uint64_t CPUBITWIDTH = 64;

uint64_t SIZEOFUINT64     = 8; // must be the same as REGISTERSIZE
uint64_t SIZEOFUINT64STAR = 8; // must be the same as REGISTERSIZE

uint64_t* power_of_two_table;

uint64_t INT64_MAX_T; // maximum numerical value of a signed 64-bit integer
uint64_t INT64_MIN_T; // minimum numerical value of a signed 64-bit integer

uint64_t UINT64_MAX_T; // maximum numerical value of an unsigned 64-bit integer

uint64_t MAX_FILENAME_LENGTH = 128;

uint64_t* character_buffer; // buffer for reading and writing characters
uint64_t* integer_buffer;   // buffer for printing integers
uint64_t* filename_buffer;  // buffer for opening files
uint64_t* binary_buffer;    // buffer for binary I/O

// flags for opening read-only files
// LINUX:       0 = 0x0000 = O_RDONLY (0x0000)
// MAC:         0 = 0x0000 = O_RDONLY (0x0000)
// WINDOWS: 32768 = 0x8000 = _O_BINARY (0x8000) | _O_RDONLY (0x0000)
// since LINUX/MAC do not seem to mind about _O_BINARY set
// we use the WINDOWS flags as default
uint64_t O_RDONLY = 32768;

// flags for opening write-only files
// MAC: 1537 = 0x0601 = O_CREAT (0x0200) | O_TRUNC (0x0400) | O_WRONLY (0x0001)
uint64_t MAC_O_CREAT_TRUNC_WRONLY = 1537;

// LINUX: 577 = 0x0241 = O_CREAT (0x0040) | O_TRUNC (0x0200) | O_WRONLY (0x0001)
uint64_t LINUX_O_CREAT_TRUNC_WRONLY = 577;

// WINDOWS: 33537 = 0x8301 = _O_BINARY (0x8000) | _O_CREAT (0x0100) | _O_TRUNC (0x0200) | _O_WRONLY (0x0001)
uint64_t WINDOWS_O_BINARY_CREAT_TRUNC_WRONLY = 33537;

// flags for rw-r--r-- file permissions
// 420 = 00644 = S_IRUSR (00400) | S_IWUSR (00200) | S_IRGRP (00040) | S_IROTH (00004)
// these flags seem to be working for LINUX, MAC, and WINDOWS
uint64_t S_IRUSR_IWUSR_IRGRP_IROTH = 420;

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t number_of_written_characters = 0;

uint64_t* output_name = (uint64_t*) 0;
uint64_t  output_fd   = 1; // 1 is file descriptor of standard output

// ------------------------- INITIALIZATION ------------------------

void init_library() {
  uint64_t i;

  // powers of two table with CPUBITWIDTH entries for 2^0 to 2^(CPUBITWIDTH - 1)
  power_of_two_table = smalloc(CPUBITWIDTH * SIZEOFUINT64);

  *power_of_two_table = 1; // 2^0 == 1

  i = 1;

  while (i < CPUBITWIDTH) {
    // compute powers of two incrementally using this recurrence relation
    *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;

    i = i + 1;
  }

  // compute 64-bit unsigned integer range using signed integer arithmetic
  UINT64_MAX_T = -1;

  // compute 64-bit signed integer range using unsigned integer arithmetic
  INT64_MAX_T = two_to_the_power_of(CPUBITWIDTH - 1) - 1;
  INT64_MIN_T = INT64_MAX_T + 1;

  // allocate and touch to make sure memory is mapped for read calls
  character_buffer  = smalloc(SIZEOFUINT64);
  *character_buffer = 0;

  // accommodate at least CPUBITWIDTH numbers for itoa, no mapping needed
  integer_buffer = smalloc(CPUBITWIDTH + 1);

  // does not need to be mapped
  filename_buffer = smalloc(MAX_FILENAME_LENGTH);

  // allocate and touch to make sure memory is mapped for read calls
  binary_buffer  = smalloc(SIZEOFUINT64);
  *binary_buffer = 0;
}

void reset_library() {
  number_of_written_characters = 0;
}

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t line_number = 1; // current line number for error reporting

uint64_t* identifier = (uint64_t*) 0; // stores scanned identifier as string
uint64_t* integer    = (uint64_t*) 0; // stores scanned integer as string
uint64_t* string     = (uint64_t*) 0; // stores scanned string

uint64_t literal = 0; // stores numerical value of scanned integer or character

uint64_t integer_is_signed = 0; // enforce INT64_MIN limit if '-' was scanned before

uint64_t character; // most recently read character

uint64_t number_of_read_characters = 0;

uint64_t symbol; // most recently recognized symbol

uint64_t number_of_ignored_characters = 0;
uint64_t number_of_comments           = 0;
uint64_t number_of_scanned_symbols    = 0;

uint64_t* source_name = (uint64_t*) 0; // name of source file
uint64_t  source_fd   = 0;             // file descriptor of open source file

uint64_t number_of_global_variables = 0;
uint64_t number_of_procedures       = 0;
uint64_t number_of_strings          = 0;

uint64_t number_of_searches = 0;
uint64_t total_search_time  = 0;

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// -------------------     I N T E R F A C E     -------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- REGISTER ---------------------------
// -----------------------------------------------------------------

void init_register();

uint64_t* get_register_name(uint64_t reg);
void      print_register_name(uint64_t reg);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t NUMBEROFREGISTERS   = 32;
uint64_t NUMBEROFTEMPORARIES = 7;

uint64_t REG_ZR  = 0;
uint64_t REG_RA  = 1;
uint64_t REG_SP  = 2;
uint64_t REG_GP  = 3;
uint64_t REG_TP  = 4;
uint64_t REG_T0  = 5;
uint64_t REG_T1  = 6;
uint64_t REG_T2  = 7;
uint64_t REG_FP  = 8;
uint64_t REG_S1  = 9;
uint64_t REG_A0  = 10;
uint64_t REG_A1  = 11;
uint64_t REG_A2  = 12;
uint64_t REG_A3  = 13;
uint64_t REG_A4  = 14;
uint64_t REG_A5  = 15;
uint64_t REG_A6  = 16;
uint64_t REG_A7  = 17;
uint64_t REG_S2  = 18;
uint64_t REG_S3  = 19;
uint64_t REG_S4  = 20;
uint64_t REG_S5  = 21;
uint64_t REG_S6  = 22;
uint64_t REG_S7  = 23;
uint64_t REG_S8  = 24;
uint64_t REG_S9  = 25;
uint64_t REG_S10 = 26;
uint64_t REG_S11 = 27;
uint64_t REG_T3  = 28;
uint64_t REG_T4  = 29;
uint64_t REG_T5  = 30;
uint64_t REG_T6  = 31;

uint64_t* REGISTERS; // strings representing registers

// ------------------------- INITIALIZATION ------------------------

void init_register() {
  REGISTERS = smalloc(NUMBEROFREGISTERS * SIZEOFUINT64STAR);

  *(REGISTERS + REG_ZR)  = (uint64_t) "$zero";
  *(REGISTERS + REG_RA)  = (uint64_t) "$ra";
  *(REGISTERS + REG_SP)  = (uint64_t) "$sp";
  *(REGISTERS + REG_GP)  = (uint64_t) "$gp";
  *(REGISTERS + REG_TP)  = (uint64_t) "$tp";
  *(REGISTERS + REG_T0)  = (uint64_t) "$t0";
  *(REGISTERS + REG_T1)  = (uint64_t) "$t1";
  *(REGISTERS + REG_T2)  = (uint64_t) "$t2";
  *(REGISTERS + REG_FP)  = (uint64_t) "$fp";
  *(REGISTERS + REG_S1)  = (uint64_t) "$s1";
  *(REGISTERS + REG_A0)  = (uint64_t) "$a0";
  *(REGISTERS + REG_A1)  = (uint64_t) "$a1";
  *(REGISTERS + REG_A2)  = (uint64_t) "$a2";
  *(REGISTERS + REG_A3)  = (uint64_t) "$a3";
  *(REGISTERS + REG_A4)  = (uint64_t) "$a4";
  *(REGISTERS + REG_A5)  = (uint64_t) "$a5";
  *(REGISTERS + REG_A6)  = (uint64_t) "$a6";
  *(REGISTERS + REG_A7)  = (uint64_t) "$a7";
  *(REGISTERS + REG_S2)  = (uint64_t) "$s2";
  *(REGISTERS + REG_S3)  = (uint64_t) "$s3";
  *(REGISTERS + REG_S4)  = (uint64_t) "$s4";
  *(REGISTERS + REG_S5)  = (uint64_t) "$s5";
  *(REGISTERS + REG_S6)  = (uint64_t) "$s6";
  *(REGISTERS + REG_S7)  = (uint64_t) "$s7";
  *(REGISTERS + REG_S8)  = (uint64_t) "$s8";
  *(REGISTERS + REG_S9)  = (uint64_t) "$s9";
  *(REGISTERS + REG_S10) = (uint64_t) "$s10";
  *(REGISTERS + REG_S11) = (uint64_t) "$s11";
  *(REGISTERS + REG_T3)  = (uint64_t) "$t3";
  *(REGISTERS + REG_T4)  = (uint64_t) "$t4";
  *(REGISTERS + REG_T5)  = (uint64_t) "$t5";
  *(REGISTERS + REG_T6)  = (uint64_t) "$t6";
}

// -----------------------------------------------------------------
// ------------------------ ENCODER/DECODER ------------------------
// -----------------------------------------------------------------

// void check_immediate_range(uint64_t found, uint64_t bits);

uint64_t encode_r_format(uint64_t funct7, uint64_t rs2, uint64_t rs1, uint64_t funct3, uint64_t rd, uint64_t opcode);
uint64_t get_funct7(uint64_t instruction);
uint64_t get_rs2(uint64_t instruction);
uint64_t get_rs1(uint64_t instruction);
uint64_t get_funct3(uint64_t instruction);
uint64_t get_rd(uint64_t instruction);
uint64_t get_opcode(uint64_t instruction);
void     decode_r_format();

uint64_t encode_i_format(uint64_t immediate, uint64_t rs1, uint64_t funct3, uint64_t rd, uint64_t opcode);
uint64_t get_immediate_i_format(uint64_t instruction);
void     decode_i_format();

uint64_t encode_s_format(uint64_t immediate, uint64_t rs2, uint64_t rs1, uint64_t funct3, uint64_t opcode);
uint64_t get_immediate_s_format(uint64_t instruction);
void     decode_s_format();

uint64_t encode_b_format(uint64_t immediate, uint64_t rs2, uint64_t rs1, uint64_t funct3, uint64_t opcode);
uint64_t get_immediate_b_format(uint64_t instruction);
void     decode_b_format();

uint64_t encode_j_format(uint64_t immediate, uint64_t rd, uint64_t opcode);
uint64_t get_immediate_j_format(uint64_t instruction);
void     decode_j_format();

uint64_t encode_u_format(uint64_t immediate, uint64_t rd, uint64_t opcode);
uint64_t get_immediate_u_format(uint64_t instruction);
void     decode_u_format();

// ------------------------ GLOBAL CONSTANTS -----------------------

// opcodes
uint64_t OP_LD     = 3;   // 0000011, I format (LD)
uint64_t OP_IMM    = 19;  // 0010011, I format (ADDI, NOP)
uint64_t OP_SD     = 35;  // 0100011, S format (SD)
uint64_t OP_OP     = 51;  // 0110011, R format (ADD, SUB, MUL, DIVU, REMU, SLTU)
uint64_t OP_LUI    = 55;  // 0110111, U format (LUI)
uint64_t OP_BRANCH = 99;  // 1100011, B format (BEQ)
uint64_t OP_JALR   = 103; // 1100111, I format (JALR)
uint64_t OP_JAL    = 111; // 1101111, J format (JAL)
uint64_t OP_SYSTEM = 115; // 1110011, I format (ECALL)

// f3-codes
uint64_t F3_NOP   = 0; // 000
uint64_t F3_ADDI  = 0; // 000
uint64_t F3_ADD   = 0; // 000
uint64_t F3_SUB   = 0; // 000
uint64_t F3_MUL   = 0; // 000
uint64_t F3_DIVU  = 5; // 101
uint64_t F3_REMU  = 7; // 111
uint64_t F3_SLTU  = 3; // 011
uint64_t F3_LD    = 3; // 011
uint64_t F3_SD    = 3; // 011
uint64_t F3_BEQ   = 0; // 000
uint64_t F3_JALR  = 0; // 000
uint64_t F3_ECALL = 0; // 000

// f7-codes
uint64_t F7_ADD  = 0;  // 0000000
uint64_t F7_MUL  = 1;  // 0000001
uint64_t F7_SUB  = 32; // 0100000
uint64_t F7_DIVU = 1;  // 0000001
uint64_t F7_REMU = 1;  // 0000001
uint64_t F7_SLTU = 0;  // 0000000

// f12-codes (immediates)
uint64_t F12_ECALL = 0; // 000000000000

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t opcode = 0;
uint64_t rs1    = 0;
uint64_t rs2    = 0;
uint64_t rd     = 0;
uint64_t imm    = 0;
uint64_t funct3 = 0;
uint64_t funct7 = 0;

// -----------------------------------------------------------------
// ---------------------------- BINARY -----------------------------
// -----------------------------------------------------------------

void reset_instruction_counters();

uint64_t get_total_number_of_instructions();

void print_instruction_counter(uint64_t total, uint64_t counter, uint64_t* mnemonics);
void print_instruction_counters();

uint64_t load_instruction(uint64_t baddr);
void     store_instruction(uint64_t baddr, uint64_t instruction);

uint64_t load_data(uint64_t baddr);
void     store_data(uint64_t baddr, uint64_t data);

uint64_t* create_elf_header(uint64_t binary_length);
uint64_t  validate_elf_header(uint64_t* header);

uint64_t open_write_only(uint64_t* name);

uint64_t* touch(uint64_t* memory, uint64_t length);

void selfie_load();

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t MAX_BINARY_LENGTH = 262144; // 256KB = MAX_CODE_LENGTH + MAX_DATA_LENGTH

uint64_t MAX_CODE_LENGTH = 245760; // 240KB
uint64_t MAX_DATA_LENGTH = 16384; // 16KB

uint64_t ELF_HEADER_LEN = 120; // = 64 + 56 bytes (file + program header)

// according to RISC-V pk
uint64_t ELF_ENTRY_POINT = 65536; // = 0x10000 (address of beginning of code)

// ------------------------ GLOBAL VARIABLES -----------------------

// instruction counters

uint64_t ic_lui   = 0;
uint64_t ic_addi  = 0;
uint64_t ic_add   = 0;
uint64_t ic_sub   = 0;
uint64_t ic_mul   = 0;
uint64_t ic_divu  = 0;
uint64_t ic_remu  = 0;
uint64_t ic_sltu  = 0;
uint64_t ic_ld    = 0;
uint64_t ic_sd    = 0;
uint64_t ic_beq   = 0;
uint64_t ic_jal   = 0;
uint64_t ic_jalr  = 0;
uint64_t ic_ecall = 0;

uint64_t* binary        = (uint64_t*) 0; // binary of code and data segments
uint64_t  binary_length = 0; // length of binary in bytes including data segment
uint64_t* binary_name   = (uint64_t*) 0; // file name of binary

uint64_t code_length = 0; // length of code segment in binary in bytes
uint64_t entry_point = 0; // beginning of code segment in virtual address space

uint64_t* code_line_number = (uint64_t*) 0; // code line number per emitted instruction
uint64_t* data_line_number = (uint64_t*) 0; // data line number per emitted data

uint64_t* assembly_name = (uint64_t*) 0; // name of assembly file
uint64_t  assembly_fd   = 0; // file descriptor of open assembly file

uint64_t* ELF_header = (uint64_t*) 0;

// -----------------------------------------------------------------
// ----------------------- MIPSTER SYSCALLS ------------------------
// -----------------------------------------------------------------

void implement_exit(uint64_t* context);

void implement_read(uint64_t* context);

void implement_write(uint64_t* context);

uint64_t down_load_string(uint64_t* table, uint64_t vstring, uint64_t* s);
void     implement_open(uint64_t* context);

void implement_brk(uint64_t* context);

void implement_assert_begin(uint64_t* context);
void implement_assert(uint64_t* context);
void implement_assert_end(uint64_t* context);

void implement_symbolic_input(uint64_t* context);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t debug_read  = 0;
uint64_t debug_write = 0;
uint64_t debug_open  = 0;
uint64_t debug_brk   = 0;

uint64_t SYSCALL_EXIT   = 93;
uint64_t SYSCALL_READ   = 63;
uint64_t SYSCALL_WRITE  = 64;
uint64_t SYSCALL_OPEN   = 1024;
uint64_t SYSCALL_BRK    = 214;
uint64_t SYSCALL_SYMPOLIC_INPUT  = 42;
uint64_t SYSCALL_ASSERT_ZONE_BGN = 44;
uint64_t SYSCALL_ASSERT          = 45;
uint64_t SYSCALL_ASSERT_ZONE_END = 46;

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ----------------------    R U N T I M E    ----------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- MEMORY -----------------------------
// -----------------------------------------------------------------

void init_memory(uint64_t megabytes);

uint64_t load_physical_memory(uint64_t* paddr);
void     store_physical_memory(uint64_t* paddr, uint64_t data);

uint64_t frame_for_page(uint64_t* table, uint64_t page);
uint64_t get_frame_for_page(uint64_t* table, uint64_t page);
uint64_t is_page_mapped(uint64_t* table, uint64_t page);

uint64_t is_valid_virtual_address(uint64_t vaddr);
uint64_t get_page_of_virtual_address(uint64_t vaddr);
uint64_t is_virtual_address_mapped(uint64_t* table, uint64_t vaddr);

uint64_t* tlb(uint64_t* table, uint64_t vaddr);

uint64_t load_virtual_memory(uint64_t* table, uint64_t vaddr);
void     store_virtual_memory(uint64_t* table, uint64_t vaddr, uint64_t data);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t debug_tlb = 0;

uint64_t MEGABYTE = 1048576; // 1MB

uint64_t VIRTUALMEMORYSIZE = 4294967296; // 4GB of virtual memory

uint64_t WORDSIZE       = 4; // in bytes
uint64_t WORDSIZEINBITS = 32;

uint64_t INSTRUCTIONSIZE = 4; // must be the same as WORDSIZE
uint64_t REGISTERSIZE    = 8; // must be twice of WORDSIZE

uint64_t PAGESIZE = 4096; // we use standard 4KB pages

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t page_frame_memory = 0; // size of memory for frames

// ------------------------- INITIALIZATION ------------------------

void init_memory(uint64_t megabytes) {
  if (megabytes > 4096)
    megabytes = 4096;

  page_frame_memory = megabytes * MEGABYTE;
}

// -----------------------------------------------------------------
// ------------------------- INSTRUCTIONS --------------------------
// -----------------------------------------------------------------

void print_code_line_number_for_instruction(uint64_t a);
void print_code_context_for_instruction(uint64_t a);

void print_lui();
void print_lui_before();
void print_lui_after();
void do_lui();

void print_addi();
void print_addi_before();
void print_addi_add_sub_mul_divu_remu_sltu_after();
void do_addi();

void print_add_sub_mul_divu_remu_sltu(uint64_t *mnemonics);
void print_add_sub_mul_divu_remu_sltu_before();

void do_add();

void do_sub();

void do_mul();

void record_divu_remu();
void do_divu();

void do_remu();

void do_sltu();

void     print_ld();
void     print_ld_before();
void     print_ld_after(uint64_t vaddr);
uint64_t do_ld();

void     print_sd();
void     print_sd_before();
void     print_sd_after(uint64_t vaddr);
uint64_t do_sd();
void     undo_sd();

void print_beq();
void print_beq_before();
void print_beq_after();
void do_beq();

void print_jal();
void print_jal_before();
void print_jal_jalr_after();
void do_jal();

void print_jalr();
void print_jalr_before();
void do_jalr();

void print_ecall();
void do_ecall();

void print_data_line_number();
void print_data_context(uint64_t data);
void print_data(uint64_t data);

// -----------------------------------------------------------------
// ------------------- SYMBOLIC EXECUTION ENGINE -------------------
// -----------------------------------------------------------------

// uint64_t is_safe_address(uint64_t vaddr, uint64_t reg);
uint64_t load_symbolic_memory(uint64_t* pt, uint64_t vaddr);

uint64_t fuzz_lo(uint64_t value);
uint64_t fuzz_up(uint64_t value);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t debug_symbolic = 0;

uint64_t fuzz = 0; // power-of-two fuzzing factor for read calls

// -----------------------------------------------------------------
// -------------------------- INTERPRETER --------------------------
// -----------------------------------------------------------------

void init_interpreter();
void reset_interpreter();

void     print_register_hexadecimal(uint64_t reg);
void     print_register_octal(uint64_t reg);
uint64_t is_system_register(uint64_t reg);
void     print_register_value(uint64_t reg);

void print_exception(uint64_t exception, uint64_t faulting_page);
void throw_exception(uint64_t exception, uint64_t faulting_page);

void fetch();
void decode_execute();
void interrupt();

uint64_t* run_until_exception();

uint64_t instruction_with_max_counter(uint64_t* counters, uint64_t max);
uint64_t print_per_instruction_counter(uint64_t total, uint64_t* counters, uint64_t max);
void     print_per_instruction_profile(uint64_t* message, uint64_t total, uint64_t* counters);

void print_profile();

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t EXCEPTION_NOEXCEPTION        = 0;
uint64_t EXCEPTION_PAGEFAULT          = 1;
uint64_t EXCEPTION_SYSCALL            = 2;
uint64_t EXCEPTION_TIMER              = 3;
uint64_t EXCEPTION_INVALIDADDRESS     = 4;
uint64_t EXCEPTION_DIVISIONBYZERO     = 5;
uint64_t EXCEPTION_UNKNOWNINSTRUCTION = 6;
uint64_t EXCEPTION_MAXTRACE           = 7;

uint64_t* EXCEPTIONS; // strings representing exceptions

uint64_t debug_exception = 0;

// enables recording, disassembling, debugging, and symbolically executing code
uint64_t debug = 0;

uint64_t execute     = 0; // flag for executing code
uint64_t record      = 0; // flag for recording code execution
uint64_t undo        = 0; // flag for undoing code execution
uint64_t redo        = 0; // flag for redoing code execution
uint64_t disassemble = 0; // flag for disassembling code
uint64_t backtrack   = 0; // flag for backtracking symbolic execution

uint64_t disassemble_verbose = 0; // flag for disassembling code in more detail

// number of instructions from context switch to timer interrupt
// CAUTION: avoid interrupting any kernel activities, keep TIMESLICE large
// TODO: implement proper interrupt controller to turn interrupts on and off
uint64_t TIMESLICE = 10000000;

uint64_t TIMEROFF = 0;

// ------------------------ GLOBAL VARIABLES -----------------------

// hardware thread state

uint64_t pc = 0; // program counter
uint64_t ir = 0; // instruction register

uint64_t* registers = (uint64_t*) 0; // general-purpose registers

uint64_t* pt = (uint64_t*) 0; // page table

// core state

uint64_t timer = 0; // counter for timer interrupt
uint64_t trap  = 0; // flag for creating a trap

// profile

uint64_t  calls               = 0;             // total number of executed procedure calls
uint64_t* calls_per_procedure = (uint64_t*) 0; // number of executed calls of each procedure

uint64_t  iterations          = 0;             // total number of executed loop iterations
uint64_t* iterations_per_loop = (uint64_t*) 0; // number of executed iterations of each loop

uint64_t* loads_per_instruction  = (uint64_t*) 0; // number of executed loads per load instruction
uint64_t* stores_per_instruction = (uint64_t*) 0; // number of executed stores per store instruction

// ------------------------- INITIALIZATION ------------------------

void init_interpreter() {
  EXCEPTIONS = smalloc((EXCEPTION_MAXTRACE + 1) * SIZEOFUINT64STAR);

  *(EXCEPTIONS + EXCEPTION_NOEXCEPTION)        = (uint64_t) "no exception";
  *(EXCEPTIONS + EXCEPTION_PAGEFAULT)          = (uint64_t) "page fault";
  *(EXCEPTIONS + EXCEPTION_SYSCALL)            = (uint64_t) "syscall";
  *(EXCEPTIONS + EXCEPTION_TIMER)              = (uint64_t) "timer interrupt";
  *(EXCEPTIONS + EXCEPTION_INVALIDADDRESS)     = (uint64_t) "invalid address";
  *(EXCEPTIONS + EXCEPTION_DIVISIONBYZERO)     = (uint64_t) "division by zero";
  *(EXCEPTIONS + EXCEPTION_UNKNOWNINSTRUCTION) = (uint64_t) "unknown instruction";
  *(EXCEPTIONS + EXCEPTION_MAXTRACE)           = (uint64_t) "trace length exceeded";
}

void reset_interpreter() {
  pc = 0;
  ir = 0;

  registers = (uint64_t*) 0;

  pt = (uint64_t*) 0;

  trap = 0;

  timer = TIMEROFF;

  if (execute) {
    reset_instruction_counters();

    calls               = 0;
    calls_per_procedure = zalloc(MAX_CODE_LENGTH / INSTRUCTIONSIZE * SIZEOFUINT64);

    iterations          = 0;
    iterations_per_loop = zalloc(MAX_CODE_LENGTH / INSTRUCTIONSIZE * SIZEOFUINT64);

    loads_per_instruction  = zalloc(MAX_CODE_LENGTH / INSTRUCTIONSIZE * SIZEOFUINT64);
    stores_per_instruction = zalloc(MAX_CODE_LENGTH / INSTRUCTIONSIZE * SIZEOFUINT64);
  }
}

// -----------------------------------------------------------------
// ---------------------------- CONTEXTS ---------------------------
// -----------------------------------------------------------------

uint64_t* allocate_context(uint64_t* parent, uint64_t* vctxt, uint64_t* in);
void      free_context(uint64_t* context);
uint64_t* delete_context(uint64_t* context, uint64_t* from);

// context struct:
// +----+----------------+
// |  0 | next context    | pointer to next context
// |  1 | prev context    | pointer to previous context
// |  2 | program counter | program counter
// |  3 | regs            | pointer to general purpose registers
// |  4 | page table      | pointer to page table
// |  5 | lo page         | lowest low unmapped page
// |  6 | me page         | highest low unmapped page
// |  7 | hi page         | highest high unmapped page
// |  8 | original break  | original end of data segment
// |  9 | program break   | end of data segment
// | 10 | exception       | exception ID
// | 11 | faulting page   | faulting page
// | 12 | exit code       | exit code
// | 13 | parent          | context that created this context
// | 14 | virtual context | virtual context address
// | 15 | name            | binary name loaded into context
// +----+-----------------+

uint64_t next_context(uint64_t* context)    { return (uint64_t) context; }
uint64_t prev_context(uint64_t* context)    { return (uint64_t) (context + 1); }
uint64_t program_counter(uint64_t* context) { return (uint64_t) (context + 2); }
uint64_t regs(uint64_t* context)            { return (uint64_t) (context + 3); }
uint64_t page_table(uint64_t* context)      { return (uint64_t) (context + 4); }
uint64_t lo_page(uint64_t* context)         { return (uint64_t) (context + 5); }
uint64_t me_page(uint64_t* context)         { return (uint64_t) (context + 6); }
uint64_t hi_page(uint64_t* context)         { return (uint64_t) (context + 7); }
uint64_t original_break(uint64_t* context)  { return (uint64_t) (context + 8); }
uint64_t program_break(uint64_t* context)   { return (uint64_t) (context + 9); }
uint64_t exception(uint64_t* context)       { return (uint64_t) (context + 10); }
uint64_t faulting_page(uint64_t* context)   { return (uint64_t) (context + 11); }
uint64_t exit_code(uint64_t* context)       { return (uint64_t) (context + 12); }
uint64_t parent(uint64_t* context)          { return (uint64_t) (context + 13); }
uint64_t virtual_context(uint64_t* context) { return (uint64_t) (context + 14); }
uint64_t name(uint64_t* context)            { return (uint64_t) (context + 15); }

uint64_t* get_next_context(uint64_t* context)    { return (uint64_t*) *context; }
uint64_t* get_prev_context(uint64_t* context)    { return (uint64_t*) *(context + 1); }
uint64_t  get_pc(uint64_t* context)              { return             *(context + 2); }
uint64_t* get_regs(uint64_t* context)            { return (uint64_t*) *(context + 3); }
uint64_t* get_pt(uint64_t* context)              { return (uint64_t*) *(context + 4); }
uint64_t  get_lo_page(uint64_t* context)         { return             *(context + 5); }
uint64_t  get_me_page(uint64_t* context)         { return             *(context + 6); }
uint64_t  get_hi_page(uint64_t* context)         { return             *(context + 7); }
uint64_t  get_original_break(uint64_t* context)  { return             *(context + 8); }
uint64_t  get_program_break(uint64_t* context)   { return             *(context + 9); }
uint64_t  get_exception(uint64_t* context)       { return             *(context + 10); }
uint64_t  get_faulting_page(uint64_t* context)   { return             *(context + 11); }
uint64_t  get_exit_code(uint64_t* context)       { return             *(context + 12); }
uint64_t* get_parent(uint64_t* context)          { return (uint64_t*) *(context + 13); }
uint64_t* get_virtual_context(uint64_t* context) { return (uint64_t*) *(context + 14); }
uint64_t* get_name(uint64_t* context)            { return (uint64_t*) *(context + 15); }

void set_next_context(uint64_t* context, uint64_t* next)     { *context        = (uint64_t) next; }
void set_prev_context(uint64_t* context, uint64_t* prev)     { *(context + 1)  = (uint64_t) prev; }
void set_pc(uint64_t* context, uint64_t pc)                  { *(context + 2)  = pc; }
void set_regs(uint64_t* context, uint64_t* regs)             { *(context + 3)  = (uint64_t) regs; }
void set_pt(uint64_t* context, uint64_t* pt)                 { *(context + 4)  = (uint64_t) pt; }
void set_lo_page(uint64_t* context, uint64_t lo_page)        { *(context + 5)  = lo_page; }
void set_me_page(uint64_t* context, uint64_t me_page)        { *(context + 6)  = me_page; }
void set_hi_page(uint64_t* context, uint64_t hi_page)        { *(context + 7)  = hi_page; }
void set_original_break(uint64_t* context, uint64_t brk)     { *(context + 8)  = brk; }
void set_program_break(uint64_t* context, uint64_t brk)      { *(context + 9)  = brk; }
void set_exception(uint64_t* context, uint64_t exception)    { *(context + 10) = exception; }
void set_faulting_page(uint64_t* context, uint64_t page)     { *(context + 11) = page; }
void set_exit_code(uint64_t* context, uint64_t code)         { *(context + 12) = code; }
void set_parent(uint64_t* context, uint64_t* parent)         { *(context + 13) = (uint64_t) parent; }
void set_virtual_context(uint64_t* context, uint64_t* vctxt) { *(context + 14) = (uint64_t) vctxt; }
void set_name(uint64_t* context, uint64_t* name)             { *(context + 15) = (uint64_t) name; }

// -----------------------------------------------------------------
// -------------------------- MICROKERNEL --------------------------
// -----------------------------------------------------------------

void reset_microkernel();

uint64_t* create_context(uint64_t* parent, uint64_t* vctxt);

void map_page(uint64_t* context, uint64_t page, uint64_t frame);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t debug_create = 0;
uint64_t debug_map    = 0;

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t* current_context = (uint64_t*) 0; // context currently running

uint64_t* used_contexts = (uint64_t*) 0; // doubly-linked list of used contexts
uint64_t* free_contexts = (uint64_t*) 0; // singly-linked list of free contexts

// ------------------------- INITIALIZATION ------------------------

void reset_microkernel() {
  current_context = (uint64_t*) 0;

  while (used_contexts != (uint64_t*) 0)
    used_contexts = delete_context(used_contexts, used_contexts);
}

// -----------------------------------------------------------------
// ---------------------------- KERNEL -----------------------------
// -----------------------------------------------------------------

uint64_t pavailable();
uint64_t pexcess();
uint64_t pused();

uint64_t* palloc();
void      pfree(uint64_t* frame);

void map_and_store(uint64_t* context, uint64_t vaddr, uint64_t data);

void up_load_binary(uint64_t* context);

uint64_t up_load_string(uint64_t* context, uint64_t* s, uint64_t SP);
void     up_load_arguments(uint64_t* context, uint64_t argc, uint64_t* argv);

uint64_t handle_system_call(uint64_t* context);
uint64_t handle_page_fault(uint64_t* context);
uint64_t handle_division_by_zero(uint64_t* context);
uint64_t handle_max_trace(uint64_t* context);
uint64_t handle_timer(uint64_t* context);

uint64_t handle_exception(uint64_t* context);

uint64_t engine(uint64_t* to_context);

// uint64_t is_boot_level_zero();

uint64_t selfie_run(uint64_t machine);

// ------------------------ GLOBAL CONSTANTS -----------------------

uint64_t* MY_CONTEXT = (uint64_t*) 0;

uint64_t DONOTEXIT = 0;
uint64_t EXIT      = 1;

uint64_t EXITCODE_NOERROR                = 0;
uint64_t EXITCODE_BADARGUMENTS           = 1;
uint64_t EXITCODE_IOERROR                = 2;
uint64_t EXITCODE_SCANNERERROR           = 3;
uint64_t EXITCODE_PARSERERROR            = 4;
uint64_t EXITCODE_COMPILERERROR          = 5;
uint64_t EXITCODE_OUTOFVIRTUALMEMORY     = 6;
uint64_t EXITCODE_OUTOFPHYSICALMEMORY    = 7;
uint64_t EXITCODE_DIVISIONBYZERO         = 8;
uint64_t EXITCODE_UNKNOWNINSTRUCTION     = 9;
uint64_t EXITCODE_UNKNOWNSYSCALL         = 10;
uint64_t EXITCODE_MULTIPLEEXCEPTIONERROR = 11;
uint64_t EXITCODE_SYMBOLICEXECUTIONERROR = 12;
uint64_t EXITCODE_OUTOFTRACEMEMORY       = 13;
uint64_t EXITCODE_UNCAUGHTEXCEPTION      = 14;

uint64_t SYSCALL_BITWIDTH = 32; // integer bit width for system calls

uint64_t MIPSTER = 1;
uint64_t DIPSTER = 2;
uint64_t RIPSTER = 3;

uint64_t MONSTER = 4;

uint64_t MINSTER = 5;
uint64_t MOBSTER = 6;

uint64_t HYPSTER = 7;

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t next_page_frame = 0;

uint64_t allocated_page_frame_memory = 0;
uint64_t free_page_frame_memory      = 0;

// -----------------------------------------------------------------
// ----------------------------- MAIN ------------------------------
// -----------------------------------------------------------------

void init_selfie(uint64_t argc, uint64_t* argv);

uint64_t  number_of_remaining_arguments();
uint64_t* remaining_arguments();

uint64_t* peek_argument();
uint64_t* get_argument();
void      set_argument(uint64_t* argv);

void print_usage();

// ------------------------ GLOBAL VARIABLES -----------------------

uint64_t  selfie_argc = 0;
uint64_t* selfie_argv = (uint64_t*) 0;

uint64_t* exe_name = (uint64_t*) 0;

// ------------------------- INITIALIZATION ------------------------

void init_selfie(uint64_t argc, uint64_t* argv) {
  selfie_argc = argc;
  selfie_argv = argv;

  exe_name = get_argument();
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     L I B R A R Y     ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ----------------------- LIBRARY PROCEDURES ----------------------
// -----------------------------------------------------------------

uint64_t two_to_the_power_of(uint64_t p) {
  // assert: 0 <= p < CPUBITWIDTH
  return *(power_of_two_table + p);
}

uint64_t ten_to_the_power_of(uint64_t p) {
  // use recursion for simplicity and educational value
  // for p close to 0 performance is not relevant
  if (p == 0)
    return 1;
  else
    return ten_to_the_power_of(p - 1) * 10;
}

uint64_t left_shift(uint64_t n, uint64_t b) {
  // assert: 0 <= b < CPUBITWIDTH
  return n * two_to_the_power_of(b);
}

uint64_t right_shift(uint64_t n, uint64_t b) {
  // assert: 0 <= b < CPUBITWIDTH
  return n / two_to_the_power_of(b);
}

uint64_t get_bits(uint64_t n, uint64_t i, uint64_t b) {
  // assert: 0 < b <= i + b < CPUBITWIDTH
  if (i == 0)
    return n % two_to_the_power_of(b);
  else
    // shift to-be-loaded bits all the way to the left
    // to reset all bits to the left of them, then
    // shift to-be-loaded bits all the way to the right and return
    return right_shift(left_shift(n, CPUBITWIDTH - (i + b)), CPUBITWIDTH - b);
}

uint64_t get_low_word(uint64_t n) {
  return get_bits(n, 0, WORDSIZEINBITS);
}

uint64_t get_high_word(uint64_t n) {
  return get_bits(n, WORDSIZEINBITS, WORDSIZEINBITS);
}

uint64_t abs(uint64_t n) {
  if (signed_less_than(n, 0))
    return -n;
  else
    return n;
}

uint64_t signed_less_than(uint64_t a, uint64_t b) {
  // INT64_MIN <= n <= INT64_MAX iff
  // INT64_MIN + INT64_MIN <= n + INT64_MIN <= INT64_MAX + INT64_MIN iff
  // -2^64 <= n + INT64_MIN <= 2^64 - 1 (sign-extended to 65 bits) iff
  // 0 <= n + INT64_MIN <= UINT64_MAX
  return a + INT64_MIN_T < b + INT64_MIN_T;
}

uint64_t signed_division(uint64_t a, uint64_t b) {
  // assert: b != 0
  // assert: a == INT64_MIN -> b != -1
  if (a == INT64_MIN_T)
    if (b == INT64_MIN_T)
      return 1;
    else if (signed_less_than(b, 0))
      return INT64_MIN_T / abs(b);
    else
      return -(INT64_MIN_T / b);
  else if (b == INT64_MIN_T)
    return 0;
  else if (signed_less_than(a, 0))
    if (signed_less_than(b, 0))
      return abs(a) / abs(b);
    else
      return -(abs(a) / b);
  else if (signed_less_than(b, 0))
    return -(a / abs(b));
  else
    return a / b;
}

uint64_t is_signed_integer(uint64_t n, uint64_t b) {
  // assert: 0 < b <= CPUBITWIDTH
  if (n < two_to_the_power_of(b - 1))
    // assert: 0 <= n < 2^(b - 1)
    return 1;
  else if (n >= -two_to_the_power_of(b - 1))
    // assert: -2^(b - 1) <= n < 2^64
    return 1;
  else
    return 0;
}

uint64_t sign_extend(uint64_t n, uint64_t b) {
  // assert: 0 <= n <= 2^b
  // assert: 0 < b < CPUBITWIDTH
  if (n < two_to_the_power_of(b - 1))
    return n;
  else
    return n - two_to_the_power_of(b);
}

uint64_t sign_shrink(uint64_t n, uint64_t b) {
  // assert: -2^(b - 1) <= n < 2^(b - 1)
  // assert: 0 < b < CPUBITWIDTH
  return get_bits(n, 0, b);
}

uint64_t load_character(uint64_t* s, uint64_t i) {
  // assert: i >= 0
  uint64_t a;

  // a is the index of the double word where
  // the to-be-loaded i-th character in s is
  a = i / SIZEOFUINT64;

  // return i-th 8-bit character in s
  return get_bits(*(s + a), (i % SIZEOFUINT64) * 8, 8);
}

uint64_t* store_character(uint64_t* s, uint64_t i, uint64_t c) {
  // assert: i >= 0, 0 <= c < 2^8 (all characters are 8-bit)
  uint64_t a;

  // a is the index of the double word where
  // the with c to-be-overwritten i-th character in s is
  a = i / SIZEOFUINT64;

  // subtract the to-be-overwritten character to reset its bits in s
  // then add c to set its bits at the i-th position in s
  *(s + a) = (*(s + a) - left_shift(load_character(s, i), (i % SIZEOFUINT64) * 8)) + left_shift(c, (i % SIZEOFUINT64) * 8);

  return s;
}

uint64_t string_length(uint64_t* s) {
  uint64_t i;

  i = 0;

  while (load_character(s, i) != 0)
    i = i + 1;

  return i;
}

void string_reverse(uint64_t* s) {
  uint64_t i;
  uint64_t j;
  uint64_t tmp;

  i = 0;
  j = string_length(s) - 1;

  while (i < j) {
    tmp = load_character(s, i);

    store_character(s, i, load_character(s, j));
    store_character(s, j, tmp);

    i = i + 1;
    j = j - 1;
  }
}

uint64_t string_compare(uint64_t* s, uint64_t* t) {
  uint64_t i;

  i = 0;

  while (1)
    if (load_character(s, i) == 0)
      if (load_character(t, i) == 0)
        return 1;
      else
        return 0;
    else if (load_character(s, i) == load_character(t, i))
      i = i + 1;
    else
      return 0;
}

uint64_t atoi(uint64_t* s) {
  uint64_t i;
  uint64_t n;
  uint64_t c;

  // the conversion of the ASCII string in s to its
  // numerical value n begins with the leftmost digit in s
  i = 0;

  // and the numerical value 0 for n
  n = 0;

  // load character (one byte) at index i in s from memory requires
  // bit shifting since memory access can only be done in double words
  c = load_character(s, i);

  // loop until s is terminated
  while (c != 0) {
    // the numerical value of ASCII-encoded decimal digits
    // is offset by the ASCII code of '0' (which is 48)
    c = c - '0';

    if (c > 9) {
      printf2((uint64_t*) "%s: cannot convert non-decimal number %s\n", exe_name, s);

      exit(EXITCODE_BADARGUMENTS);
    }

    // assert: s contains a decimal number

    // use base 10 but detect wrap around
    if (n < UINT64_MAX_T / 10)
      n = n * 10 + c;
    else if (n == UINT64_MAX_T / 10)
      if (c <= UINT64_MAX_T % 10)
        n = n * 10 + c;
      else {
        // s contains a decimal number larger than UINT64_MAX
        printf2((uint64_t*) "%s: cannot convert out-of-bound number %s\n", exe_name, s);

        exit(EXITCODE_BADARGUMENTS);
      }
    else {
      // s contains a decimal number larger than UINT64_MAX
      printf2((uint64_t*) "%s: cannot convert out-of-bound number %s\n", exe_name, s);

      exit(EXITCODE_BADARGUMENTS);
    }

    // go to the next digit
    i = i + 1;

    // load character (one byte) at index i in s from memory requires
    // bit shifting since memory access can only be done in double words
    c = load_character(s, i);
  }

  return n;
}

uint64_t* itoa(uint64_t n, uint64_t* s, uint64_t b, uint64_t a) {
  // assert: b in {2,4,8,10,16}

  uint64_t i;
  uint64_t sign;

  // the conversion of the integer n to an ASCII string in s with
  // base b and alignment a begins with the leftmost digit in s
  i = 0;

  // for now assuming n is positive
  sign = 0;

  if (n == 0) {
    store_character(s, 0, '0');

    i = 1;
  } else if (signed_less_than(n, 0)) {
    if (b == 10) {
      // n is represented as two's complement
      // convert n to a positive number but remember the sign
      n = -n;

      sign = 1;
    }
  }

  while (n != 0) {
    if (n % b > 9)
      // the ASCII code of hexadecimal digits larger than 9
      // is offset by the ASCII code of 'A' (which is 65)
      store_character(s, i, n % b - 10 + 'A');
    else
      // the ASCII code of digits less than or equal to 9
      // is offset by the ASCII code of '0' (which is 48)
      store_character(s, i, n % b + '0');

    // convert n by dividing n with base b
    n = n / b;

    i = i + 1;
  }

  if (b == 10) {
    if (sign) {
      store_character(s, i, '-'); // negative decimal numbers start with -

      i = i + 1;
    }

    while (i < a) {
      store_character(s, i, ' '); // align with spaces

      i = i + 1;
    }
  } else {
    while (i < a) {
      store_character(s, i, '0'); // align with 0s

      i = i + 1;
    }

    if (b == 8) {
      store_character(s, i, '0'); // octal numbers start with 00
      store_character(s, i + 1, '0');

      i = i + 2;
    } else if (b == 16) {
      store_character(s, i, 'x'); // hexadecimal numbers start with 0x
      store_character(s, i + 1, '0');

      i = i + 2;
    }
  }

  store_character(s, i, 0); // null-terminated string

  // our numeral system is positional hindu-arabic, that is,
  // the weight of digits increases right to left, which means
  // that we need to reverse the string we computed above
  string_reverse(s);

  return s;
}

uint64_t fixed_point_ratio(uint64_t a, uint64_t b, uint64_t f) {
  // compute fixed point ratio with f fractional digits
  // multiply a/b with 10^f but avoid wrap around

  uint64_t p;

  p = f;

  while (p > 0) {
    if (a <= UINT64_MAX_T / ten_to_the_power_of(p)) {
      if (b / ten_to_the_power_of(f - p) != 0)
        return (a * ten_to_the_power_of(p)) / (b / ten_to_the_power_of(f - p));
    }

    p = p - 1;
  }

  return 0;
}

uint64_t fixed_point_percentage(uint64_t r, uint64_t f) {
  if (r != 0)
    // 10^4 (for 100.00%) * 10^f (for f fractional digits of r)
    return ten_to_the_power_of(4 + f) / r;
  else
    return 0;
}

void put_character(uint64_t c) {
  *character_buffer = c;

  // assert: character_buffer is mapped

  // try to write 1 character from character_buffer
  // into file with output_fd file descriptor
  if (write(output_fd, character_buffer, 1) == 1) {
    if (output_fd != 1)
      // count number of characters written to a file,
      // not the console which has file descriptor 1
      number_of_written_characters = number_of_written_characters + 1;
  } else {
    // write failed
    if (output_fd != 1) {
      // failed write was not to the console which has file descriptor 1
      // to report the error we may thus still write to the console
      output_fd = 1;

      printf2((uint64_t*) "%s: could not write character to output file %s\n", exe_name, output_name);
    }

    exit(EXITCODE_IOERROR);
  }
}

void print(uint64_t* s) {
  uint64_t i;

  if (s == (uint64_t*) 0)
    print((uint64_t*) "NULL");
  else {
    i = 0;

    while (load_character(s, i) != 0) {
      put_character(load_character(s, i));

      i = i + 1;
    }
  }
}

void println() {
  put_character(CHAR_LF);
}

void print_character(uint64_t c) {
  put_character(CHAR_SINGLEQUOTE);

  if (c == CHAR_EOF)
    print((uint64_t*) "end of file");
  else if (c == CHAR_TAB)
    print((uint64_t*) "tabulator");
  else if (c == CHAR_LF)
    print((uint64_t*) "line feed");
  else if (c == CHAR_CR)
    print((uint64_t*) "carriage return");
  else
    put_character(c);

  put_character(CHAR_SINGLEQUOTE);
}

void print_string(uint64_t* s) {
  put_character(CHAR_DOUBLEQUOTE);

  print(s);

  put_character(CHAR_DOUBLEQUOTE);
}

void print_integer(uint64_t n) {
  print(itoa(n, integer_buffer, 10, 0));
}

void unprint_integer(uint64_t n) {
  n = string_length(itoa(n, integer_buffer, 10, 0));

  while (n > 0) {
    put_character(CHAR_BACKSPACE);

    n = n - 1;
  }
}

void print_hexadecimal(uint64_t n, uint64_t a) {
  print(itoa(n, integer_buffer, 16, a));
}

void print_octal(uint64_t n, uint64_t a) {
  print(itoa(n, integer_buffer, 8, a));
}

void print_binary(uint64_t n, uint64_t a) {
  print(itoa(n, integer_buffer, 2, a));
}

uint64_t print_format0(uint64_t* s, uint64_t i) {
  // print string s from index i on
  // ignore % formatting codes except for %%
  if (s == (uint64_t*) 0)
    return 0;
  else {
    while (load_character(s, i) != 0) {
      if (load_character(s, i) != '%') {
        put_character(load_character(s, i));

        i = i + 1;
      } else if (load_character(s, i + 1) == '%') {
        // for %% print just one %
        put_character('%');

        i = i + 2;
      } else {
        put_character(load_character(s, i));

        i = i + 1;
      }
    }

    return i;
  }
}

uint64_t print_format1(uint64_t* s, uint64_t i, uint64_t* a) {
  // print string s from index i on until next % formatting code except for %%
  // then print argument a according to the encountered % formatting code

  uint64_t p;

  if (s == (uint64_t*) 0)
    return 0;
  else {
    while (load_character(s, i) != 0) {
      if (load_character(s, i) != '%') {
        put_character(load_character(s, i));

        i = i + 1;
      } else if (load_character(s, i + 1) == 's') {
        print(a);

        return i + 2;
      } else if (load_character(s, i + 1) == 'c') {
        put_character((uint64_t) a);

        return i + 2;
      } else if (load_character(s, i + 1) == 'd') {
        print_integer((uint64_t) a);

        return i + 2;
      } else if (load_character(s, i + 1) == '.') {
        // for simplicity we support a single digit only
        p = load_character(s, i + 2) - '0';

        if (p < 10) {
          // the character at i + 2 is in fact a digit
          print_integer((uint64_t) a / ten_to_the_power_of(p));

          if (p > 0) {
            // using integer_buffer here is ok since we are not using print_integer
            itoa((uint64_t) a % ten_to_the_power_of(p), integer_buffer, 10, 0);
            p = p - string_length(integer_buffer);

            put_character('.');
            while (p > 0) {
              put_character('0');

              p = p - 1;
            }
            print(integer_buffer);
          }

          return i + 4;
        } else {
          put_character(load_character(s, i));

          i = i + 1;
        }
      } else if (load_character(s, i + 1) == 'p') {
        print_hexadecimal((uint64_t) a, SIZEOFUINT64STAR);

        return i + 2;
      } else if (load_character(s, i + 1) == 'x') {
        print_hexadecimal((uint64_t) a, 0);

        return i + 2;
      } else if (load_character(s, i + 1) == 'o') {
        print_octal((uint64_t) a, 0);

        return i + 2;
      } else if (load_character(s, i + 1) == 'b') {
        print_binary((uint64_t) a, 0);

        return i + 2;
      } else if (load_character(s, i + 1) == '%') {
        // for %% print just one %
        put_character('%');

        i = i + 2;
      } else {
        put_character(load_character(s, i));

        i = i + 1;
      }
    }

    return i;
  }
}

void printf1(uint64_t* s, uint64_t* a1) {
  print_format0(s, print_format1(s, 0, a1));
}

void printf2(uint64_t* s, uint64_t* a1, uint64_t* a2) {
  print_format0(s, print_format1(s, print_format1(s, 0, a1), a2));
}

void printf3(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3) {
  print_format0(s, print_format1(s, print_format1(s, print_format1(s, 0, a1), a2), a3));
}

void printf4(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3, uint64_t* a4) {
  print_format0(s, print_format1(s, print_format1(s, print_format1(s, print_format1(s, 0, a1), a2), a3), a4));
}

void printf5(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3, uint64_t* a4, uint64_t* a5) {
  print_format0(s, print_format1(s, print_format1(s, print_format1(s, print_format1(s, print_format1(s, 0, a1), a2), a3), a4), a5));
}

void printf6(uint64_t* s, uint64_t* a1, uint64_t* a2, uint64_t* a3, uint64_t* a4, uint64_t* a5, uint64_t* a6) {
  print_format0(s, print_format1(s, print_format1(s, print_format1(s, print_format1(s, print_format1(s, print_format1(s, 0, a1), a2), a3), a4), a5), a6));
}

uint64_t round_up(uint64_t n, uint64_t m) {
  if (n % m == 0)
    return n;
  else
    return n - n % m + m;
}

uint64_t* smalloc(uint64_t size) {
  // this procedure ensures a defined program exit,
  // if no memory can be allocated
  uint64_t* memory;

  memory = malloc(size);

  if (size == 0)
    // any address including null
    return memory;
  else if ((uint64_t) memory == 0) {
    printf1((uint64_t*) "%s: malloc out of memory\n", exe_name);

    exit(EXITCODE_OUTOFVIRTUALMEMORY);
  }

  return memory;
}

uint64_t* zalloc(uint64_t size) {
  // this procedure is only executed at boot level zero
  // zalloc allocates size bytes rounded up to word size
  // and then zeroes that memory, similar to calloc, but
  // called zalloc to avoid redeclaring calloc
  uint64_t* memory;
  uint64_t  i;

  size = round_up(size, REGISTERSIZE);

  memory = smalloc(size);

  size = size / REGISTERSIZE;

  i = 0;

  while (i < size) {
    // erase memory by setting it to 0
    *(memory + i) = 0;

    i = i + 1;
  }

  return memory;
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// -------------------     I N T E R F A C E     -------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// --------------------------- REGISTER ----------------------------
// -----------------------------------------------------------------

uint64_t* get_register_name(uint64_t reg) {
  return (uint64_t*) *(REGISTERS + reg);
}

void print_register_name(uint64_t reg) {
  print(get_register_name(reg));
}

// -----------------------------------------------------------------
// ------------------------ ENCODER/DECODER ------------------------
// -----------------------------------------------------------------

// RISC-V R Format
// ----------------------------------------------------------------
// |        7         |  5  |  5  |  3   |        5        |  7   |
// +------------------+-----+-----+------+-----------------+------+
// |      funct7      | rs2 | rs1 |funct3|       rd        |opcode|
// +------------------+-----+-----+------+-----------------+------+
// |31              25|24 20|19 15|14  12|11              7|6    0|
// ----------------------------------------------------------------

uint64_t get_funct7(uint64_t instruction) {
  return get_bits(instruction, 25, 7);
}

uint64_t get_rs2(uint64_t instruction) {
  return get_bits(instruction, 20, 5);
}

uint64_t get_rs1(uint64_t instruction) {
  return get_bits(instruction, 15, 5);
}

uint64_t get_funct3(uint64_t instruction) {
  return get_bits(instruction, 12, 3);
}

uint64_t get_rd(uint64_t instruction) {
  return get_bits(instruction, 7, 5);
}

uint64_t get_opcode(uint64_t instruction) {
  return get_bits(instruction, 0, 7);
}

void decode_r_format() {
  funct7 = get_funct7(ir);
  rs2    = get_rs2(ir);
  rs1    = get_rs1(ir);
  funct3 = get_funct3(ir);
  rd     = get_rd(ir);
  imm    = 0;
}

// RISC-V I Format
// ----------------------------------------------------------------
// |           12           |  5  |  3   |        5        |  7   |
// +------------------------+-----+------+-----------------+------+
// |    immediate[11:0]     | rs1 |funct3|       rd        |opcode|
// +------------------------+-----+------+-----------------+------+
// |31                    20|19 15|14  12|11              7|6    0|
// ----------------------------------------------------------------

uint64_t get_immediate_i_format(uint64_t instruction) {
  return sign_extend(get_bits(instruction, 20, 12), 12);
}

void decode_i_format() {
  funct7 = 0;
  rs2    = 0;
  rs1    = get_rs1(ir);
  funct3 = get_funct3(ir);
  rd     = get_rd(ir);
  imm    = get_immediate_i_format(ir);
}

// RISC-V S Format
// ----------------------------------------------------------------
// |        7         |  5  |  5  |  3   |        5        |  7   |
// +------------------+-----+-----+------+-----------------+------+
// |    imm1[11:5]    | rs2 | rs1 |funct3|    imm2[4:0]    |opcode|
// +------------------+-----+-----+------+-----------------+------+
// |31              25|24 20|19 15|14  12|11              7|6    0|
// ----------------------------------------------------------------

uint64_t get_immediate_s_format(uint64_t instruction) {
  uint64_t imm1;
  uint64_t imm2;

  imm1 = get_bits(instruction, 25, 7);
  imm2 = get_bits(instruction,  7, 5);

  return sign_extend(left_shift(imm1, 5) + imm2, 12);
}

void decode_s_format() {
  funct7 = 0;
  rs2    = get_rs2(ir);
  rs1    = get_rs1(ir);
  funct3 = get_funct3(ir);
  rd     = 0;
  imm    = get_immediate_s_format(ir);
}

// RISC-V B Format
// ----------------------------------------------------------------
// |        7         |  5  |  5  |  3   |        5        |  7   |
// +------------------+-----+-----+------+-----------------+------+
// |imm1[12]imm2[10:5]| rs2 | rs1 |funct3|imm3[4:1]imm4[11]|opcode|
// +------------------+-----+-----+------+-----------------+------+
// |31              25|24 20|19 15|14  12|11              7|6    0|
// ----------------------------------------------------------------

uint64_t get_immediate_b_format(uint64_t instruction) {
  uint64_t imm1;
  uint64_t imm2;
  uint64_t imm3;
  uint64_t imm4;

  imm1 = get_bits(instruction, 31, 1);
  imm2 = get_bits(instruction, 25, 6);
  imm3 = get_bits(instruction,  8, 4);
  imm4 = get_bits(instruction,  7, 1);

  // reassemble immediate and add trailing zero
  return sign_extend(left_shift(left_shift(left_shift(left_shift(imm1, 1) + imm4, 6) + imm2, 4) + imm3, 1), 13);
}

void decode_b_format() {
  funct7 = 0;
  rs2    = get_rs2(ir);
  rs1    = get_rs1(ir);
  funct3 = get_funct3(ir);
  rd     = 0;
  imm    = get_immediate_b_format(ir);
}

// RISC-V J Format
// ----------------------------------------------------------------
// |                  20                 |        5        |  7   |
// +-------------------------------------+-----------------+------+
// |imm1[20]imm2[10:1]imm3[11]imm4[19:12]|       rd        |opcode|
// +-------------------------------------+-----------------+------+
// |31                                 12|11              7|6    0|
// ----------------------------------------------------------------

uint64_t get_immediate_j_format(uint64_t instruction) {
  uint64_t imm1;
  uint64_t imm2;
  uint64_t imm3;
  uint64_t imm4;

  imm1 = get_bits(instruction, 31,  1);
  imm2 = get_bits(instruction, 21, 10);
  imm3 = get_bits(instruction, 20,  1);
  imm4 = get_bits(instruction, 12,  8);

  // reassemble immediate and add trailing zero
  return sign_extend(left_shift(left_shift(left_shift(left_shift(imm1, 8) + imm4, 1) + imm3, 10) + imm2, 1), 21);
}

void decode_j_format() {
  funct7 = 0;
  rs2    = 0;
  rs1    = 0;
  funct3 = 0;
  rd     = get_rd(ir);
  imm    = get_immediate_j_format(ir);
}

// RISC-V U Format
// ----------------------------------------------------------------
// |                  20                 |        5        |  7   |
// +-------------------------------------+-----------------+------+
// |           immediate[19:0]           |       rd        |opcode|
// +-------------------------------------+-----------------+------+
// |31                                 12|11              7|6    0|
// ----------------------------------------------------------------

uint64_t get_immediate_u_format(uint64_t instruction) {
  return sign_extend(get_bits(instruction, 12, 20), 20);
}

void decode_u_format() {
  funct7 = 0;
  rs2    = 0;
  rs1    = 0;
  funct3 = 0;
  rd     = get_rd(ir);
  imm    = get_immediate_u_format(ir);
}

// -----------------------------------------------------------------
// ---------------------------- BINARY -----------------------------
// -----------------------------------------------------------------

void reset_instruction_counters() {
  ic_lui   = 0;
  ic_addi  = 0;
  ic_add   = 0;
  ic_sub   = 0;
  ic_mul   = 0;
  ic_divu  = 0;
  ic_remu  = 0;
  ic_sltu  = 0;
  ic_ld    = 0;
  ic_sd    = 0;
  ic_beq   = 0;
  ic_jal   = 0;
  ic_jalr  = 0;
  ic_ecall = 0;
}

uint64_t get_total_number_of_instructions() {
  return ic_lui + ic_addi + ic_add + ic_sub + ic_mul + ic_divu + ic_remu + ic_sltu + ic_ld + ic_sd + ic_beq + ic_jal + ic_jalr + ic_ecall;
}

void print_instruction_counter(uint64_t total, uint64_t counter, uint64_t* mnemonics) {
  printf3((uint64_t*)
    "%s: %d(%.2d%%)",
    mnemonics,
    (uint64_t*) counter,
    (uint64_t*) fixed_point_percentage(fixed_point_ratio(total, counter, 4), 4));
}

void print_instruction_counters() {
  uint64_t ic;

  ic = get_total_number_of_instructions();

  printf1((uint64_t*) "%s: init:    ", exe_name);
  print_instruction_counter(ic, ic_lui, (uint64_t*) "lui");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_addi, (uint64_t*) "addi");
  println();

  printf1((uint64_t*) "%s: memory:  ", exe_name);
  print_instruction_counter(ic, ic_ld, (uint64_t*) "ld");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_sd, (uint64_t*) "sd");
  println();

  printf1((uint64_t*) "%s: compute: ", exe_name);
  print_instruction_counter(ic, ic_add, (uint64_t*) "add");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_sub, (uint64_t*) "sub");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_mul, (uint64_t*) "mul");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_divu, (uint64_t*) "divu");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_remu, (uint64_t*) "remu");
  println();

  printf1((uint64_t*) "%s: control: ", exe_name);
  print_instruction_counter(ic, ic_sltu, (uint64_t*) "sltu");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_beq, (uint64_t*) "beq");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_jal, (uint64_t*) "jal");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_jalr, (uint64_t*) "jalr");
  print((uint64_t*) ", ");
  print_instruction_counter(ic, ic_ecall, (uint64_t*) "ecall");
  println();
}

uint64_t load_instruction(uint64_t baddr) {
  if (baddr % REGISTERSIZE == 0)
    return get_low_word(*(binary + baddr / REGISTERSIZE));
  else
    return get_high_word(*(binary + baddr / REGISTERSIZE));
}

uint64_t load_data(uint64_t baddr) {
  return *(binary + baddr / REGISTERSIZE);
}

uint64_t* create_elf_header(uint64_t binary_length) {
  uint64_t* header;

  // store all numbers necessary to create a minimal and valid
  // ELF64 header including the program header
  header = smalloc(ELF_HEADER_LEN);

  // RISC-U ELF64 file header:
  *(header + 0) = 127                               // magic number part 0 is 0x7F
                + left_shift((uint64_t) 'E', 8)     // magic number part 1
                + left_shift((uint64_t) 'L', 16)    // magic number part 2
                + left_shift((uint64_t) 'F', 24)    // magic number part 3
                + left_shift(2, 32)                 // file class is ELFCLASS64
                + left_shift(1, 40)                 // object file data structures endianess is ELFDATA2LSB
                + left_shift(1, 48);                // version of the object file format
  *(header + 1) = 0;                                // ABI version and start of padding bytes
  *(header + 2) = 2                                 // object file type is ET_EXEC
                + left_shift(243, 16)               // target architecture is RV64
                + left_shift(1, 32);                // version of the object file format
  *(header + 3) = ELF_ENTRY_POINT;                  // entry point address
  *(header + 4) = 8 * SIZEOFUINT64;                 // program header offset
  *(header + 5) = 0;                                // section header offset
  *(header + 6) = left_shift(8 * SIZEOFUINT64, 32)  // elf header size
                + left_shift(7 * SIZEOFUINT64, 48); // size of program header entry
  *(header + 7) = 1;                                // number of program header entries

  // RISC-U ELF64 program header table:
  *(header + 8)  = 1                              // type of segment is LOAD
                 + left_shift(7, 32);             // segment attributes is RWX
  *(header + 9)  = ELF_HEADER_LEN + SIZEOFUINT64; // segment offset in file
  *(header + 10) = ELF_ENTRY_POINT;               // virtual address in memory
  *(header + 11) = 0;                             // physical address (reserved)
  *(header + 12) = binary_length;                 // size of segment in file
  *(header + 13) = binary_length;                 // size of segment in memory
  *(header + 14) = PAGESIZE;                      // alignment of segment

  return header;
}

uint64_t validate_elf_header(uint64_t* header) {
  uint64_t  new_entry_point;
  uint64_t  new_binary_length;
  uint64_t  position;
  uint64_t* valid_header;

  new_entry_point   = *(header + 10);
  new_binary_length = *(header + 12);

  if (new_binary_length != *(header + 13))
    // segment size in file is not the same as segment size in memory
    return 0;

  if (new_entry_point > VIRTUALMEMORYSIZE - PAGESIZE - new_binary_length)
    // binary does not fit into virtual address space
    return 0;

  valid_header = create_elf_header(new_binary_length);

  position = 0;

  while (position < ELF_HEADER_LEN / SIZEOFUINT64) {
    if (*(header + position) != *(valid_header + position))
      return 0;

    position = position + 1;
  }

  entry_point   = new_entry_point;
  binary_length = new_binary_length;

  return 1;
}

uint64_t open_write_only(uint64_t* name) {
  // we try opening write-only files using platform-specific flags
  // to make selfie platform-independent, this may nevertheless
  // not always work and require intervention
  uint64_t fd;

  // try Mac flags
  fd = sign_extend(open(name, MAC_O_CREAT_TRUNC_WRONLY, S_IRUSR_IWUSR_IRGRP_IROTH), SYSCALL_BITWIDTH);

  if (signed_less_than(fd, 0)) {
    // try Linux flags
    fd = sign_extend(open(name, LINUX_O_CREAT_TRUNC_WRONLY, S_IRUSR_IWUSR_IRGRP_IROTH), SYSCALL_BITWIDTH);

    if (signed_less_than(fd, 0))
      // try Windows flags
      fd = sign_extend(open(name, WINDOWS_O_BINARY_CREAT_TRUNC_WRONLY, S_IRUSR_IWUSR_IRGRP_IROTH), SYSCALL_BITWIDTH);
  }

  return fd;
}

uint64_t* touch(uint64_t* memory, uint64_t length) {
  uint64_t* m;
  uint64_t n;

  m = memory;

  if (length > 0)
    // touch memory at beginning
    n = *m;

  while (length > PAGESIZE) {
    length = length - PAGESIZE;

    m = m + PAGESIZE / REGISTERSIZE;

    // touch every following page
    n = *m;
  }

  if (length > 0) {
    m = m + (length - 1) / REGISTERSIZE;

    // touch at end
    n = *m;
  }

  // avoids unused warning for n
  n = 0; n = n + 1;

  return memory;
}

void selfie_load() {
  uint64_t fd;
  uint64_t number_of_read_bytes;

  binary_name = get_argument();

  // assert: binary_name is mapped and not longer than MAX_FILENAME_LENGTH

  fd = sign_extend(open(binary_name, O_RDONLY, 0), SYSCALL_BITWIDTH);

  if (signed_less_than(fd, 0)) {
    printf2((uint64_t*) "%s: could not open input file %s\n", exe_name, binary_name);

    exit(EXITCODE_IOERROR);
  }

  // make sure binary is mapped for reading into it
  binary = touch(smalloc(MAX_BINARY_LENGTH), MAX_BINARY_LENGTH);

  binary_length = 0;
  code_length   = 0;
  entry_point   = 0;

  // no source line numbers in binaries
  code_line_number = (uint64_t*) 0;
  data_line_number = (uint64_t*) 0;

  // make sure ELF_header is mapped for reading into it
  ELF_header = touch(smalloc(ELF_HEADER_LEN), ELF_HEADER_LEN);

  // read ELF_header first
  number_of_read_bytes = read(fd, ELF_header, ELF_HEADER_LEN);

  if (number_of_read_bytes == ELF_HEADER_LEN) {
    if (validate_elf_header(ELF_header)) {
      // now read code length
      number_of_read_bytes = read(fd, binary_buffer, SIZEOFUINT64);

      if (number_of_read_bytes == SIZEOFUINT64) {
        code_length = *binary_buffer;

        if (binary_length <= MAX_BINARY_LENGTH) {
          // now read binary including global variables and strings
          number_of_read_bytes = sign_extend(read(fd, binary, binary_length), SYSCALL_BITWIDTH);

          if (signed_less_than(0, number_of_read_bytes)) {
            // check if we are really at EOF
            if (read(fd, binary_buffer, SIZEOFUINT64) == 0) {
              printf5((uint64_t*) "%s: %d bytes with %d instructions and %d bytes of data loaded from %s\n",
                exe_name,
                (uint64_t*) (ELF_HEADER_LEN + SIZEOFUINT64 + binary_length),
                (uint64_t*) (code_length / INSTRUCTIONSIZE),
                (uint64_t*) (binary_length - code_length),
                binary_name);

              return;
            }
          }
        }
      }
    }
  }

  printf2((uint64_t*) "%s: failed to load code from input file %s\n", exe_name, binary_name);

  exit(EXITCODE_IOERROR);
}

// -----------------------------------------------------------------
// ----------------------- MIPSTER SYSCALLS ------------------------
// -----------------------------------------------------------------

void implement_exit(uint64_t* context) {
  if (disassemble) {
    print((uint64_t*) "(exit): ");
    print_register_hexadecimal(REG_A0);
    print((uint64_t*) " |- ->\n");
  }

  set_exit_code(context, sign_shrink(*(get_regs(context) + REG_A0), SYSCALL_BITWIDTH));

  if (sase_symbolic)
    return;

  printf4((uint64_t*)
    "%s: %s exiting with exit code %d and %.2dMB mallocated memory\n",
    exe_name,
    get_name(context),
    (uint64_t*) sign_extend(get_exit_code(context), SYSCALL_BITWIDTH),
    (uint64_t*) fixed_point_ratio(get_program_break(context) - get_original_break(context), MEGABYTE, 2));
}

void implement_read(uint64_t* context) {
  // parameters
  uint64_t fd;
  uint64_t vbuffer;
  uint64_t size;

  // local variables
  uint64_t read_total;
  uint64_t bytes_to_read;
  uint64_t failed;
  uint64_t* buffer;
  uint64_t actually_read;
  uint64_t value;
  uint64_t lo;
  uint64_t up;
  uint64_t mrvc;

  if (disassemble) {
    print((uint64_t*) "(read): ");
    print_register_value(REG_A0);
    print((uint64_t*) ",");
    print_register_hexadecimal(REG_A1);
    print((uint64_t*) ",");
    print_register_value(REG_A2);
    print((uint64_t*) " |- ");
    print_register_value(REG_A0);
  }

  fd      = *(get_regs(context) + REG_A0);
  vbuffer = *(get_regs(context) + REG_A1);
  size    = *(get_regs(context) + REG_A2);

  if (debug_read)
    printf4((uint64_t*) "%s: trying to read %d bytes from file with descriptor %d into buffer at virtual address %p\n", exe_name, (uint64_t*) size, (uint64_t*) fd, (uint64_t*) vbuffer);

  read_total   = 0;
  bytes_to_read = SIZEOFUINT64;

  failed = 0;

  while (size > 0) {
    if (is_valid_virtual_address(vbuffer)) {
      if (is_virtual_address_mapped(get_pt(context), vbuffer)) {
        buffer = tlb(get_pt(context), vbuffer);

        if (size < bytes_to_read)
          bytes_to_read = size;

        if (sase_symbolic) {
          read_buffer = vbuffer;
          if (read_tc_current < read_tc) {
            value  = concrete_reads[read_tc_current];

            // fuzz read value
            lo = fuzz_lo(value);
            up = fuzz_up(value);

            printf("reused read: %llu, lo: %llu, up: %llu, read_tc_cur: %llu, read_tc: %llu tc: %llu\n", value, lo,up, read_tc_current, read_tc, sase_tc);

            // assert: up < 2^32
            boolector_assert(btor, boolector_ulte(btor, constrained_reads[read_tc_current],boolector_unsigned_int(btor, up, bv_sort)));
            // assert: lo < 2^32
            boolector_assert(btor, boolector_ugte(btor, constrained_reads[read_tc_current],boolector_unsigned_int(btor, lo, bv_sort)));

            sase_store_memory(get_pt(context), vbuffer, SYMBOLIC_T, value, constrained_reads[read_tc_current]);
            read_tc_current++;

            actually_read = bytes_to_read;
          } else {
            // save mrvc in buffer
            mrvc = load_physical_memory(buffer);

            // caution: read only overwrites bytes_to_read number of bytes
            // we therefore need to restore the actual value in buffer
            // to preserve the original read semantics
            store_physical_memory(buffer, *(values + load_symbolic_memory(get_pt(context), vbuffer)));

            actually_read = sign_extend(read(fd, buffer, bytes_to_read), SYSCALL_BITWIDTH);

            // retrieve read value
            value = load_physical_memory(buffer);

            // restore mrvc in buffer
            store_physical_memory(buffer, mrvc);

            // fuzz read value
            lo = fuzz_lo(value);
            up = fuzz_up(value);

            if (actually_read) {
              printf("read: %llu, lo: %llu, up: %llu, read_tc_cur: %llu, read_tc: %llu, tc: %llu\n", value, lo, up,read_tc_current, read_tc, sase_tc);

              concrete_reads[read_tc] = value;

              sprintf(var_buffer, "rv_%llu", read_tc);
              // assert: up < 2^32
              constrained_reads[read_tc] = boolector_var(btor, bv_sort, var_buffer);
              boolector_assert(btor, boolector_ulte(btor, constrained_reads[read_tc], boolector_unsigned_int(btor,up, bv_sort)));
              // assert: lo < 2^32
              boolector_assert(btor, boolector_ugte(btor, constrained_reads[read_tc], boolector_unsigned_int(btor,lo, bv_sort)));

              sase_store_memory(get_pt(context), vbuffer, SYMBOLIC_T, value, constrained_reads[read_tc]);
              read_tc++;
              read_tc_current++;
            }
          }

        } else
          actually_read = sign_extend(read(fd, buffer, bytes_to_read), SYSCALL_BITWIDTH);

        if (actually_read == bytes_to_read) {
          read_total = read_total + actually_read;

          size = size - actually_read;

          if (size > 0)
            vbuffer = vbuffer + SIZEOFUINT64;
        } else {
          if (signed_less_than(0, actually_read))
            read_total = read_total + actually_read;

          size = 0;
        }
      } else {
        failed = 1;

        size = 0;

        if (debug_read)
          printf2((uint64_t*) "%s: reading into virtual address %p failed because the address is unmapped\n", exe_name, (uint64_t*) vbuffer);
      }
    } else {
      failed = 1;

      size = 0;

      if (debug_read)
        printf2((uint64_t*) "%s: reading into virtual address %p failed because the address is invalid\n", exe_name, (uint64_t*) vbuffer);
    }
  }

  if (failed == 0)
    *(get_regs(context) + REG_A0) = read_total;
  else
    *(get_regs(context) + REG_A0) = sign_shrink(-1, SYSCALL_BITWIDTH);

  if (sase_symbolic) {
    if (*(get_regs(context) + REG_A0) < two_to_the_power_of_32) {
      sase_regs[REG_A0]     = boolector_unsigned_int(btor, *(get_regs(context) + REG_A0), bv_sort);
      sase_regs_typ[REG_A0] = CONCRETE_T;
    } else
      printf2((uint64_t*) "%s: big read in read syscall: %d\n", exe_name, (uint64_t*) *(get_regs(context) +REG_A0));
  }

  set_pc(context, get_pc(context) + INSTRUCTIONSIZE);

  if (debug_read)
    printf3((uint64_t*) "%s: actually read %d bytes from file with descriptor %d\n", exe_name, (uint64_t*) read_total, (uint64_t*) fd);

  if (disassemble) {
    print((uint64_t*) " -> ");
    print_register_value(REG_A0);
    println();
  }
}

void implement_assert_begin(uint64_t* context) {
  if (sase_symbolic) {
    assert_zone = 1;
    set_pc(context, get_pc(context) + INSTRUCTIONSIZE);
  }
}

void implement_assert_end(uint64_t* context) {
  if (sase_symbolic) {
    assert_zone = 0;
    set_pc(context, get_pc(context) + INSTRUCTIONSIZE);
  }
}

void implement_assert(uint64_t* context) {
  uint64_t res = *(get_regs(context) + REG_A0);

  if (sase_symbolic) {
    if (which_branch) {
      if (res == 0) {
        printf(RED "assertion failed 1 at %llx\n" RESET, pc - entry_point);
        exit(EXITCODE_SYMBOLICEXECUTIONERROR);
      }
    } else {
      boolector_push(btor, 1);
      boolector_assert(btor, sase_false_branchs[sase_tc]);
      if (boolector_sat(btor) == BOOLECTOR_SAT) {
        printf(RED "assertion failed 2 at %llx\n" RESET, pc - entry_point);
        exit(EXITCODE_SYMBOLICEXECUTIONERROR);
      }
      boolector_pop(btor, 1);
    }

    which_branch = 0;

    set_pc(context, get_pc(context) + INSTRUCTIONSIZE);
  } else {
    print(exe_name);
    print((uint64_t*) ": symbolic input syscall during concrete execution ");
    println();
    exit(EXITCODE_SYMBOLICEXECUTIONERROR);
  }
}

void implement_symbolic_input(uint64_t* context) {
  uint64_t lo;
  uint64_t up;
  uint64_t step;

  lo   = *(get_regs(context) + REG_A0);
  up   = *(get_regs(context) + REG_A1);
  step = *(get_regs(context) + REG_A2);

  if (sase_symbolic) {
    printf("symbolic input: lo: %llu, up: %llu, step: %llu, cnt: %llu\n", lo, up, step, input_cnt_current);

    if (step > 1) {
      printf("%s\n", "step is greater than 1; continue assuming step = 1");
    }

    if (input_cnt_current < input_cnt) {
      if (up < two_to_the_power_of_32)
        boolector_assert(btor, boolector_ulte(btor, constrained_inputs[input_cnt_current], boolector_unsigned_int(btor, up, bv_sort)));
      else
        boolector_assert(btor, boolector_ulte(btor, constrained_inputs[input_cnt_current], boolector_unsigned_int_64(up)));
      // >= lo
      if (lo < two_to_the_power_of_32)
        boolector_assert(btor, boolector_ugte(btor, constrained_inputs[input_cnt_current], boolector_unsigned_int(btor, lo, bv_sort)));
      else
        boolector_assert(btor, boolector_ugte(btor, constrained_inputs[input_cnt_current], boolector_unsigned_int_64(lo)));

      sase_regs[REG_A0] = constrained_inputs[input_cnt_current];
      input_cnt_current++;

    } else {
      if (input_cnt_current > input_cnt) {
        printf("OUTPUT: input_cnt_current > input_cnt \n");
        exit(EXITCODE_SYMBOLICEXECUTIONERROR);
      }

      sprintf(var_buffer, "in_%llu", input_cnt);
      constrained_inputs[input_cnt] = boolector_var(btor, bv_sort, var_buffer);
      // <= up
      if (up < two_to_the_power_of_32)
        boolector_assert(btor, boolector_ulte(btor, constrained_inputs[input_cnt], boolector_unsigned_int(btor, up, bv_sort)));
      else
        boolector_assert(btor, boolector_ulte(btor, constrained_inputs[input_cnt], boolector_unsigned_int_64(up)));
      // >= lo
      if (lo < two_to_the_power_of_32)
        boolector_assert(btor, boolector_ugte(btor, constrained_inputs[input_cnt], boolector_unsigned_int(btor, lo, bv_sort)));
      else
        boolector_assert(btor, boolector_ugte(btor, constrained_inputs[input_cnt], boolector_unsigned_int_64(lo)));

      sase_regs[REG_A0] = constrained_inputs[input_cnt];
      input_cnt++;
      input_cnt_current++;

    }

    sase_regs_typ[REG_A0] = SYMBOLIC_T;

    set_pc(context, get_pc(context) + INSTRUCTIONSIZE);

  } else {
    print(exe_name);
    print((uint64_t*) ": symbolic input syscall during concrete execution ");
    println();
    exit(EXITCODE_SYMBOLICEXECUTIONERROR);
  }
}

void implement_write(uint64_t* context) {
  // parameters
  uint64_t fd;
  uint64_t vbuffer;
  uint64_t size;

  // local variables
  uint64_t written_total;
  uint64_t bytes_to_write;
  uint64_t failed;
  uint64_t* buffer;
  uint64_t actually_written;

  if (disassemble) {
    print((uint64_t*) "(write): ");
    print_register_value(REG_A0);
    print((uint64_t*) ",");
    print_register_hexadecimal(REG_A1);
    print((uint64_t*) ",");
    print_register_value(REG_A2);
    print((uint64_t*) " |- ");
    print_register_value(REG_A0);
  }

  fd      = *(get_regs(context) + REG_A0);
  vbuffer = *(get_regs(context) + REG_A1);
  size    = *(get_regs(context) + REG_A2);

  if (debug_write)
    printf4((uint64_t*) "%s: trying to write %d bytes from buffer at virtual address %p into file with descriptor %d\n", exe_name, (uint64_t*) size, (uint64_t*) vbuffer, (uint64_t*) fd);

  written_total = 0;
  bytes_to_write = SIZEOFUINT64;

  failed = 0;

  while (size > 0) {
    if (is_valid_virtual_address(vbuffer)) {
      if (is_virtual_address_mapped(get_pt(context), vbuffer)) {
        buffer = tlb(get_pt(context), vbuffer);

        if (size < bytes_to_write)
          bytes_to_write = size;

        if (sase_symbolic)
          // TODO: What should symbolically executed code output?
          // buffer points to a trace counter that refers to the actual value
          // actually_written = sign_extend(write(fd, values + load_physical_memory(buffer), bytes_to_write), SYSCALL_BITWIDTH);
          actually_written = bytes_to_write;
        else
          actually_written = sign_extend(write(fd, buffer, bytes_to_write), SYSCALL_BITWIDTH);

        if (actually_written == bytes_to_write) {
          written_total = written_total + actually_written;

          size = size - actually_written;

          if (size > 0)
            vbuffer = vbuffer + SIZEOFUINT64;
        } else {
          if (signed_less_than(0, actually_written))
            written_total = written_total + actually_written;

          size = 0;
        }
      } else {
        failed = 1;

        size = 0;

        if (debug_write)
          printf2((uint64_t*) "%s: writing into virtual address %p failed because the address is unmapped\n", exe_name, (uint64_t*) vbuffer);
      }
    } else {
      failed = 1;

      size = 0;

      if (debug_write)
        printf2((uint64_t*) "%s: writing into virtual address %p failed because the address is invalid\n", exe_name, (uint64_t*) vbuffer);
    }
  }

  if (failed == 0)
    *(get_regs(context) + REG_A0) = written_total;
  else
    *(get_regs(context) + REG_A0) = sign_shrink(-1, SYSCALL_BITWIDTH);

  if (sase_symbolic) {
    if (*(get_regs(context) + REG_A0) < two_to_the_power_of_32) {
      sase_regs[REG_A0]     = boolector_unsigned_int(btor, *(get_regs(context) + REG_A0), bv_sort);
      sase_regs_typ[REG_A0] = CONCRETE_T;
    } else
      printf2((uint64_t*) "%s: big write in write syscall: %d\n", exe_name, (uint64_t*) *(get_regs(context) + REG_A0));
  }

  set_pc(context, get_pc(context) + INSTRUCTIONSIZE);

  if (debug_write)
    printf3((uint64_t*) "%s: actually wrote %d bytes into file with descriptor %d\n", exe_name, (uint64_t*) written_total, (uint64_t*) fd);

  if (disassemble) {
    print((uint64_t*) " -> ");
    print_register_value(REG_A0);
    println();
  }
}

uint64_t down_load_string(uint64_t* table, uint64_t vaddr, uint64_t* s) {
  uint64_t mrvc;
  uint64_t i;
  uint64_t j;

  i = 0;

  while (i < MAX_FILENAME_LENGTH / SIZEOFUINT64) {
    if (is_valid_virtual_address(vaddr)) {
      if (is_virtual_address_mapped(table, vaddr)) {
        if (sase_symbolic) {
          mrvc = load_symbolic_memory(table, vaddr);

          *(s + i) = *(values + mrvc);

          if (*(is_symbolics + mrvc) == SYMBOLIC_T) {
            printf1((uint64_t*) "%s: detected symbolic value ", exe_name);
            print((uint64_t*) " in filename of open call\n");

            exit(EXITCODE_SYMBOLICEXECUTIONERROR);
          }
        } else
          *(s + i) = load_virtual_memory(table, vaddr);

        j = 0;

        // check if string ends in the current machine word
        while (j < SIZEOFUINT64) {
          if (load_character(s + i, j) == 0)
            return 1;

          j = j + 1;
        }

        // advance to the next machine word in virtual memory
        vaddr = vaddr + SIZEOFUINT64;

        // advance to the next machine word in our memory
        i = i + 1;
      } else if (debug_open)
        printf2((uint64_t*) "%s: opening file with name at virtual address %p failed because the address is unmapped\n", exe_name, (uint64_t*) vaddr);
    } else if (debug_open)
      printf2((uint64_t*) "%s: opening file with name at virtual address %p failed because the address is invalid\n", exe_name, (uint64_t*) vaddr);
  }

  return 0;
}

void implement_open(uint64_t* context) {
  // parameters
  uint64_t vfilename;
  uint64_t flags;
  uint64_t mode;

  // return value
  uint64_t fd;

  if (disassemble) {
    print((uint64_t*) "(open): ");
    print_register_hexadecimal(REG_A0);
    print((uint64_t*) ",");
    print_register_hexadecimal(REG_A1);
    print((uint64_t*) ",");
    print_register_octal(REG_A2);
    print((uint64_t*) " |- ");
    print_register_value(REG_A0);
  }

  vfilename = *(get_regs(context) + REG_A0);
  flags     = *(get_regs(context) + REG_A1);
  mode      = *(get_regs(context) + REG_A2);

  if (down_load_string(get_pt(context), vfilename, filename_buffer)) {
    fd = sign_extend(open(filename_buffer, flags, mode), SYSCALL_BITWIDTH);

    *(get_regs(context) + REG_A0) = fd;

    if (debug_open)
      printf5((uint64_t*) "%s: opened file %s with flags %x and mode %o returning file descriptor %d\n", exe_name, filename_buffer, (uint64_t*) flags, (uint64_t*) mode, (uint64_t*) fd);
  } else {
    *(get_regs(context) + REG_A0) = sign_shrink(-1, SYSCALL_BITWIDTH);

    if (debug_open)
      printf2((uint64_t*) "%s: opening file with name at virtual address %p failed because the name is too long\n", exe_name, (uint64_t*) vfilename);
  }

  if (sase_symbolic) {
    // assert: *(get_regs(context) + REG_A0) < 2^32
    sase_regs[REG_A0]     = boolector_unsigned_int(btor, *(get_regs(context) + REG_A0), bv_sort);
    sase_regs_typ[REG_A0] = CONCRETE_T;
  }

  set_pc(context, get_pc(context) + INSTRUCTIONSIZE);

  if (disassemble) {
    print((uint64_t*) " -> ");
    print_register_value(REG_A0);
    println();
  }
}

void implement_brk(uint64_t* context) {
  // parameter
  uint64_t program_break;

  // local variables
  uint64_t previous_program_break;
  uint64_t valid;
  uint64_t size;

  if (disassemble) {
    print((uint64_t*) "(brk): ");
    print_register_hexadecimal(REG_A0);
  }

  program_break = *(get_regs(context) + REG_A0);

  previous_program_break = get_program_break(context);

  valid = 0;

  if (program_break >= previous_program_break)
    if (program_break < *(get_regs(context) + REG_SP))
      if (program_break % SIZEOFUINT64 == 0)
        valid = 1;

  if (valid) {
    if (disassemble)
      print((uint64_t*) " |- ->\n");

    if (debug_brk)
      printf2((uint64_t*) "%s: setting program break to %p\n", exe_name, (uint64_t*) program_break);

    set_program_break(context, program_break);

    if (sase_symbolic) {
      // assert: program_break < 2^32
      // true for the original malloc code = program_break
      sase_regs[REG_A0]     = boolector_unsigned_int(btor, program_break, bv_sort); // no need
      sase_regs_typ[REG_A0] = CONCRETE_T;

      // size = program_break - previous_program_break;
      //
      // // interval is memory range, not symbolic value
      // *(reg_typ + REG_A0) = 1;
      //
      // // remember start and size of memory block for checking memory safety
      // *(reg_los + REG_A0) = previous_program_break;
      // *(reg_ups + REG_A0) = size;
    }
  } else {
    // error returns current program break
    program_break = previous_program_break;

    if (debug_brk)
      printf2((uint64_t*) "%s: retrieving current program break %p\n", exe_name, (uint64_t*) program_break);

    if (disassemble) {
      print((uint64_t*) " |- ");
      print_register_hexadecimal(REG_A0);
    }

    *(get_regs(context) + REG_A0) = program_break;

    if (disassemble) {
      print((uint64_t*) " -> ");
      print_register_hexadecimal(REG_A0);
      println();
    }

    if (sase_symbolic) {
      sase_regs[REG_A0]     = boolector_unsigned_int(btor, 0, bv_sort);
      sase_regs_typ[REG_A0] = CONCRETE_T;
    }
  }

  set_pc(context, get_pc(context) + INSTRUCTIONSIZE);
}

// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ----------------------    R U N T I M E    ----------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ---------------------------- MEMORY -----------------------------
// -----------------------------------------------------------------

uint64_t load_physical_memory(uint64_t* paddr) {
  return *paddr;
}

void store_physical_memory(uint64_t* paddr, uint64_t data) {
  *paddr = data;
}

uint64_t frame_for_page(uint64_t* table, uint64_t page) {
  return (uint64_t) (table + page);
}

uint64_t get_frame_for_page(uint64_t* table, uint64_t page) {
  return *(table + page);
}

uint64_t is_page_mapped(uint64_t* table, uint64_t page) {
  if (get_frame_for_page(table, page) != 0)
    return 1;
  else
    return 0;
}

uint64_t is_valid_virtual_address(uint64_t vaddr) {
  if (vaddr < VIRTUALMEMORYSIZE)
    // memory must be word-addressed for lack of byte-sized data type
    if (vaddr % REGISTERSIZE == 0)
      return 1;

  return 0;
}

uint64_t get_page_of_virtual_address(uint64_t vaddr) {
  return vaddr / PAGESIZE;
}

uint64_t is_virtual_address_mapped(uint64_t* table, uint64_t vaddr) {
  // assert: is_valid_virtual_address(vaddr) == 1

  return is_page_mapped(table, get_page_of_virtual_address(vaddr));
}

uint64_t* tlb(uint64_t* table, uint64_t vaddr) {
  uint64_t page;
  uint64_t frame;
  uint64_t paddr;

  // assert: is_valid_virtual_address(vaddr) == 1
  // assert: is_virtual_address_mapped(table, vaddr) == 1

  page = get_page_of_virtual_address(vaddr);

  frame = get_frame_for_page(table, page);

  // map virtual address to physical address
  paddr = vaddr - page * PAGESIZE + frame;

  if (debug_tlb)
    printf5((uint64_t*) "%s: tlb access:\n vaddr: %p\n page:  %p\n frame: %p\n paddr: %p\n", exe_name, (uint64_t*) vaddr, (uint64_t*) (page * PAGESIZE), (uint64_t*) frame, (uint64_t*) paddr);

  return (uint64_t*) paddr;
}

uint64_t load_virtual_memory(uint64_t* table, uint64_t vaddr) {
  // assert: is_valid_virtual_address(vaddr) == 1
  // assert: is_virtual_address_mapped(table, vaddr) == 1

  return load_physical_memory(tlb(table, vaddr));
}

void store_virtual_memory(uint64_t* table, uint64_t vaddr, uint64_t data) {
  // assert: is_valid_virtual_address(vaddr) == 1
  // assert: is_virtual_address_mapped(table, vaddr) == 1

  store_physical_memory(tlb(table, vaddr), data);
}

// -----------------------------------------------------------------
// ------------------------- INSTRUCTIONS --------------------------
// -----------------------------------------------------------------

void print_code_line_number_for_instruction(uint64_t a) {
  if (code_line_number != (uint64_t*) 0)
    printf1((uint64_t*) "(~%d)", (uint64_t*) *(code_line_number + a / INSTRUCTIONSIZE));
}

void print_code_context_for_instruction(uint64_t a) {
  if (execute) {
    printf2((uint64_t*) "%s: $pc=%x", binary_name, (uint64_t*) pc);
    print_code_line_number_for_instruction(pc - entry_point);
  } else {
    printf1((uint64_t*) "%x", (uint64_t*) pc);
    if (disassemble_verbose) {
      print_code_line_number_for_instruction(pc);
      printf1((uint64_t*) ": %p", (uint64_t*) ir);
    }
  }
  print((uint64_t*) ": ");
}

void print_lui() {
  print_code_context_for_instruction(pc);
  printf2((uint64_t*) "lui %s,%x", get_register_name(rd), (uint64_t*) sign_shrink(imm, 20));
}

void print_lui_before() {
  print((uint64_t*) ": |- ");
  print_register_hexadecimal(rd);
}

void print_lui_after() {
  print((uint64_t*) " -> ");
  print_register_hexadecimal(rd);
}

void do_lui() {
  // load upper immediate

  if (rd != REG_ZR)
    // semantics of lui
    *(registers + rd) = left_shift(imm, 12);

  pc = pc + INSTRUCTIONSIZE;

  ic_lui = ic_lui + 1;
}

void print_addi() {
  print_code_context_for_instruction(pc);

  if (rd == REG_ZR)
    if (rs1 == REG_ZR)
      if (imm == 0) {
        print((uint64_t*) "nop");

        return;
      }

  printf3((uint64_t*) "addi %s,%s,%d", get_register_name(rd), get_register_name(rs1), (uint64_t*) imm);
}

void print_addi_before() {
  print((uint64_t*) ": ");
  print_register_value(rs1);
  print((uint64_t*) " |- ");
  print_register_value(rd);
}

void print_addi_add_sub_mul_divu_remu_sltu_after() {
  print((uint64_t*) " -> ");
  print_register_value(rd);
}

void do_addi() {
  // add immediate

  if (rd != REG_ZR)
    // semantics of addi
    *(registers + rd) = *(registers + rs1) + imm;

  pc = pc + INSTRUCTIONSIZE;

  ic_addi = ic_addi + 1;
}

void print_add_sub_mul_divu_remu_sltu(uint64_t *mnemonics) {
  print_code_context_for_instruction(pc);
  printf4((uint64_t*) "%s %s,%s,%s", mnemonics, get_register_name(rd), get_register_name(rs1), get_register_name(rs2));
}

void print_add_sub_mul_divu_remu_sltu_before() {
  print((uint64_t*) ": ");
  print_register_value(rs1);
  print((uint64_t*) ",");
  print_register_value(rs2);
  print((uint64_t*) " |- ");
  print_register_value(rd);
}

void do_add() {
  if (rd != REG_ZR)
    // semantics of add
    *(registers + rd) = *(registers + rs1) + *(registers + rs2);

  pc = pc + INSTRUCTIONSIZE;

  ic_add = ic_add + 1;
}

void do_sub() {
  if (rd != REG_ZR)
    // semantics of sub
    *(registers + rd) = *(registers + rs1) - *(registers + rs2);

  pc = pc + INSTRUCTIONSIZE;

  ic_sub = ic_sub + 1;
}

void do_mul() {
  if (rd != REG_ZR)
    // semantics of mul
    *(registers + rd) = *(registers + rs1) * *(registers + rs2);

  // TODO: 128-bit resolution currently not supported

  pc = pc + INSTRUCTIONSIZE;

  ic_mul = ic_mul + 1;
}

void do_divu() {
  // division unsigned

  if (*(registers + rs2) != 0) {
    if (rd != REG_ZR)
      // semantics of divu
      *(registers + rd) = *(registers + rs1) / *(registers + rs2);

    pc = pc + INSTRUCTIONSIZE;

    ic_divu = ic_divu + 1;
  } else
    throw_exception(EXCEPTION_DIVISIONBYZERO, 0);
}

void do_remu() {
  // remainder unsigned

  if (*(registers + rs2) != 0) {
    if (rd != REG_ZR)
      // semantics of remu
      *(registers + rd) = *(registers + rs1) % *(registers + rs2);

    pc = pc + INSTRUCTIONSIZE;

    ic_remu = ic_remu + 1;
  } else
    throw_exception(EXCEPTION_DIVISIONBYZERO, 0);
}

void do_sltu() {
  // set on less than unsigned

  if (rd != REG_ZR) {
    // semantics of sltu
    if (*(registers + rs1) < *(registers + rs2))
      *(registers + rd) = 1;
    else
      *(registers + rd) = 0;
  }

  pc = pc + INSTRUCTIONSIZE;

  ic_sltu = ic_sltu + 1;
}

void print_ld() {
  print_code_context_for_instruction(pc);
  printf3((uint64_t*) "ld %s,%d(%s)", get_register_name(rd), (uint64_t*) imm, get_register_name(rs1));
}

void print_ld_before() {
  uint64_t vaddr;

  vaddr = *(registers + rs1) + imm;

  print((uint64_t*) ": ");
  print_register_hexadecimal(rs1);

  if (is_valid_virtual_address(vaddr))
    if (is_virtual_address_mapped(pt, vaddr)) {
      if (is_system_register(rd))
        printf2((uint64_t*) ",mem[%x]=%x |- ", (uint64_t*) vaddr, (uint64_t*) load_virtual_memory(pt, vaddr));
      else
        printf2((uint64_t*) ",mem[%x]=%d |- ", (uint64_t*) vaddr, (uint64_t*) load_virtual_memory(pt, vaddr));
      print_register_value(rd);

      return;
    }

  print((uint64_t*) " |-");
}

void print_ld_after(uint64_t vaddr) {
  if (is_valid_virtual_address(vaddr))
    if (is_virtual_address_mapped(pt, vaddr)) {
      print((uint64_t*) " -> ");
      print_register_value(rd);
      printf1((uint64_t*) "=mem[%x]", (uint64_t*) vaddr);
    }
}

uint64_t do_ld() {
  uint64_t vaddr;
  uint64_t a;

  // load double word

  vaddr = *(registers + rs1) + imm;

  if (is_valid_virtual_address(vaddr)) {
    if (is_virtual_address_mapped(pt, vaddr)) {
      if (rd != REG_ZR)
        // semantics of ld
        *(registers + rd) = load_virtual_memory(pt, vaddr);

      // keep track of instruction address for profiling loads
      a = (pc - entry_point) / INSTRUCTIONSIZE;

      pc = pc + INSTRUCTIONSIZE;

      // keep track of number of loads in total
      ic_ld = ic_ld + 1;

      // and individually
      *(loads_per_instruction + a) = *(loads_per_instruction + a) + 1;
    } else
      throw_exception(EXCEPTION_PAGEFAULT, get_page_of_virtual_address(vaddr));
  } else
    throw_exception(EXCEPTION_INVALIDADDRESS, vaddr);

  return vaddr;
}

void print_sd() {
  print_code_context_for_instruction(pc);
  printf3((uint64_t*) "sd %s,%d(%s)", get_register_name(rs2), (uint64_t*) imm, get_register_name(rs1));
}

void print_sd_before() {
  uint64_t vaddr;

  vaddr = *(registers + rs1) + imm;

  print((uint64_t*) ": ");
  print_register_hexadecimal(rs1);

  if (is_valid_virtual_address(vaddr))
    if (is_virtual_address_mapped(pt, vaddr)) {
      print((uint64_t*) ",");
      print_register_value(rs2);
      if (is_system_register(rd))
        printf2((uint64_t*) " |- mem[%x]=%x", (uint64_t*) vaddr, (uint64_t*) load_virtual_memory(pt, vaddr));
      else
        printf2((uint64_t*) " |- mem[%x]=%d", (uint64_t*) vaddr, (uint64_t*) load_virtual_memory(pt, vaddr));

      return;
    }

  print((uint64_t*) " |-");
}

void print_sd_after(uint64_t vaddr) {
  if (is_valid_virtual_address(vaddr))
    if (is_virtual_address_mapped(pt, vaddr)) {
      printf1((uint64_t*) " -> mem[%x]=", (uint64_t*) vaddr);
      print_register_value(rs2);
    }
}

uint64_t do_sd() {
  uint64_t vaddr;
  uint64_t a;

  // store double word

  vaddr = *(registers + rs1) + imm;

  if (is_valid_virtual_address(vaddr)) {
    if (is_virtual_address_mapped(pt, vaddr)) {
      // semantics of sd
      store_virtual_memory(pt, vaddr, *(registers + rs2));

      // keep track of instruction address for profiling stores
      a = (pc - entry_point) / INSTRUCTIONSIZE;

      pc = pc + INSTRUCTIONSIZE;

      // keep track of number of stores in total
      ic_sd = ic_sd + 1;

      // and individually
      *(stores_per_instruction + a) = *(stores_per_instruction + a) + 1;
    } else
      throw_exception(EXCEPTION_PAGEFAULT, get_page_of_virtual_address(vaddr));
  } else
    throw_exception(EXCEPTION_INVALIDADDRESS, vaddr);

  return vaddr;
}

void print_beq() {
  print_code_context_for_instruction(pc);
  printf4((uint64_t*) "beq %s,%s,%d[%x]", get_register_name(rs1), get_register_name(rs2), (uint64_t*) signed_division(imm, INSTRUCTIONSIZE), (uint64_t*) (pc + imm));
}

void print_beq_before() {
  print((uint64_t*) ": ");
  print_register_value(rs1);
  print((uint64_t*) ",");
  print_register_value(rs2);
  printf1((uint64_t*) " |- $pc=%x", (uint64_t*) pc);
}

void print_beq_after() {
  printf1((uint64_t*) " -> $pc=%x", (uint64_t*) pc);
}

void do_beq() {
  // branch on equal

  // semantics of beq
  if (*(registers + rs1) == *(registers + rs2))
    pc = pc + imm;
  else
    pc = pc + INSTRUCTIONSIZE;

  ic_beq = ic_beq + 1;
}

void print_jal() {
  print_code_context_for_instruction(pc);
  printf3((uint64_t*) "jal %s,%d[%x]", get_register_name(rd), (uint64_t*) signed_division(imm, INSTRUCTIONSIZE), (uint64_t*) (pc + imm));
}

void print_jal_before() {
  print((uint64_t*) ": |- ");
  if (rd != REG_ZR) {
    print_register_hexadecimal(rd);
    print((uint64_t*) ",");
  }
  printf1((uint64_t*) "$pc=%x", (uint64_t*) pc);
}

void print_jal_jalr_after() {
  print_beq_after();
  if (rd != REG_ZR) {
    print((uint64_t*) ",");
    print_register_hexadecimal(rd);
  }
}

void do_jal() {
  uint64_t a;

  // jump and link

  if (rd != REG_ZR) {
    // first link
    *(registers + rd) = pc + INSTRUCTIONSIZE;

    // then jump for procedure calls
    pc = pc + imm;

    // prologue address for profiling procedure calls
    a = (pc - entry_point) / INSTRUCTIONSIZE;

    // keep track of number of procedure calls in total
    calls = calls + 1;

    // and individually
    *(calls_per_procedure + a) = *(calls_per_procedure + a) + 1;
  } else if (signed_less_than(imm, 0)) {
    // jump backwards to check for another loop iteration
    pc = pc + imm;

    // first loop instruction address for profiling loop iterations
    a = (pc - entry_point) / INSTRUCTIONSIZE;

    // keep track of number of loop iterations in total
    iterations = iterations + 1;

    // and individually
    *(iterations_per_loop + a) = *(iterations_per_loop + a) + 1;
  } else
    // just jump forward
    pc = pc + imm;

  ic_jal = ic_jal + 1;
}

void print_jalr() {
  print_code_context_for_instruction(pc);
  printf3((uint64_t*) "jalr %s,%d(%s)", get_register_name(rd), (uint64_t*) signed_division(imm, INSTRUCTIONSIZE), get_register_name(rs1));
}

void print_jalr_before() {
  print((uint64_t*) ": ");
  print_register_hexadecimal(rs1);
  print((uint64_t*) " |- ");
  if (rd != REG_ZR) {
    print_register_hexadecimal(rd);
    print((uint64_t*) ",");
  }
  printf1((uint64_t*) "$pc=%x", (uint64_t*) pc);
}

void do_jalr() {
  uint64_t next_pc;

  // jump and link register

  if (rd == REG_ZR)
    // fast path: just return by jumping rs1-relative with LSB reset
    pc = left_shift(right_shift(*(registers + rs1) + imm, 1), 1);
  else {
    // slow path: first prepare jump, then link, just in case rd == rs1

    // prepare jump with LSB reset
    next_pc = left_shift(right_shift(*(registers + rs1) + imm, 1), 1);

    // link to next instruction
    *(registers + rd) = pc + INSTRUCTIONSIZE;

    // jump
    pc = next_pc;
  }

  ic_jalr = ic_jalr + 1;
}

void print_ecall() {
  print_code_context_for_instruction(pc);
  print((uint64_t*) "ecall");
}

void do_ecall() {
  ic_ecall = ic_ecall + 1;

  // all system calls other than switch are handled by exception
  throw_exception(EXCEPTION_SYSCALL, 0);
}

void print_data_line_number() {
  if (data_line_number != (uint64_t*) 0)
    printf1((uint64_t*) "(~%d)", (uint64_t*) *(data_line_number + (pc - code_length) / REGISTERSIZE));
}

void print_data_context(uint64_t data) {
  printf1((uint64_t*) "%x", (uint64_t*) pc);

  if (disassemble_verbose) {
    print_data_line_number();
    print((uint64_t*) ": ");
    print_hexadecimal(data, SIZEOFUINT64 * 2);
    print((uint64_t*) " ");
  } else
    print((uint64_t*) ": ");
}

void print_data(uint64_t data) {
  print_data_context(data);
  printf1((uint64_t*) ".quad %x", (uint64_t*) data);
}

// -----------------------------------------------------------------
// ------------------- SYMBOLIC EXECUTION ENGINE -------------------
// -----------------------------------------------------------------

// uint64_t is_safe_address(uint64_t vaddr, uint64_t reg) {
//   if (*(reg_typ + reg)) {
//     if (vaddr < *(reg_los + reg))
//       // memory access below start address of mallocated block
//       return 0;
//     else if (vaddr - *(reg_los + reg) >= *(reg_ups + reg))
//       // memory access above end address of mallocated block
//       return 0;
//     else
//       return 1;
//   } else if (*(reg_los + reg) == *(reg_ups + reg))
//     return 1;
//   else {
//     printf2((uint64_t*) "%s: detected unsupported symbolic access of memory interval at %x", exe_name, (uint64_t*) pc);
//     print_code_line_number_for_instruction(pc - entry_point);
//     println();
//
//     exit(EXITCODE_SYMBOLICEXECUTIONERROR);
//   }
// }

uint64_t load_symbolic_memory(uint64_t* pt, uint64_t vaddr) {
  uint64_t mrvc;

  // assert: vaddr is valid and mapped
  mrvc = load_virtual_memory(pt, vaddr);

  if (mrvc <= tc)
    return mrvc;
  else {
    printf4((uint64_t*) "%s: detected most recent value counter %d at vaddr %x greater than current trace counter %d\n", exe_name, (uint64_t*) mrvc, (uint64_t*) vaddr, (uint64_t*) tc);

    exit(EXITCODE_SYMBOLICEXECUTIONERROR);
  }
}

uint64_t fuzz_lo(uint64_t value) {
  if (fuzz >= CPUBITWIDTH)
    return 0;
  else if (value > (two_to_the_power_of(fuzz) - 1) / 2)
    return value - (two_to_the_power_of(fuzz) - 1) / 2;
  else
    return 0;
}

uint64_t fuzz_up(uint64_t value) {
  if (fuzz >= CPUBITWIDTH)
    return UINT64_MAX_T;
  else if (UINT64_MAX_T - value < two_to_the_power_of(fuzz) / 2)
    return UINT64_MAX_T;
  else if (value > (two_to_the_power_of(fuzz) - 1) / 2)
    return value + two_to_the_power_of(fuzz) / 2;
  else
    return two_to_the_power_of(fuzz) - 1;
}

// -----------------------------------------------------------------
// -------------------------- INTERPRETER --------------------------
// -----------------------------------------------------------------

void print_register_hexadecimal(uint64_t reg) {
  printf2((uint64_t*) "%s=%x", get_register_name(reg), (uint64_t*) *(registers + reg));
}

void print_register_octal(uint64_t reg) {
  printf2((uint64_t*) "%s=%o", get_register_name(reg), (uint64_t*) *(registers + reg));
}

uint64_t is_system_register(uint64_t reg) {
  if (reg == REG_GP)
    return 1;
  else if (reg == REG_FP)
    return 1;
  else if (reg == REG_RA)
    return 1;
  else if (reg == REG_SP)
    return 1;
  else
    return 0;
}

void print_register_value(uint64_t reg) {
  if (is_system_register(reg))
    print_register_hexadecimal(reg);
  else
    printf3((uint64_t*) "%s=%d(%x)", get_register_name(reg), (uint64_t*) *(registers + reg), (uint64_t*) *(registers + reg));
}

void print_exception(uint64_t exception, uint64_t faulting_page) {
  print((uint64_t*) *(EXCEPTIONS + exception));

  if (exception == EXCEPTION_PAGEFAULT)
    printf1((uint64_t*) " at %p", (uint64_t*) faulting_page);
}

void throw_exception(uint64_t exception, uint64_t faulting_page) {
  if (get_exception(current_context) != EXCEPTION_NOEXCEPTION)
    if (get_exception(current_context) != exception) {
      printf2((uint64_t*) "%s: context %p throws ", exe_name, current_context);
      print_exception(exception, faulting_page);
      print((uint64_t*) " exception in presence of ");
      print_exception(get_exception(current_context), get_faulting_page(current_context));
      print((uint64_t*) " exception\n");

      exit(EXITCODE_MULTIPLEEXCEPTIONERROR);
    }

  set_exception(current_context, exception);
  set_faulting_page(current_context, faulting_page);

  trap = 1;

  if (debug_exception) {
    printf2((uint64_t*) "%s: context %p throws ", exe_name, current_context);
    print_exception(exception, faulting_page);
    print((uint64_t*) " exception\n");
  }
}

void fetch() {
  // assert: is_valid_virtual_address(pc) == 1
  // assert: is_virtual_address_mapped(pt, pc) == 1

  if (pc % REGISTERSIZE == 0)
    ir = get_low_word(load_virtual_memory(pt, pc));
  else
    ir = get_high_word(load_virtual_memory(pt, pc - INSTRUCTIONSIZE));
}

void decode_execute() {
  opcode = get_opcode(ir);

  if (opcode == OP_IMM) {
    decode_i_format();

    if (funct3 == F3_ADDI) {
      if (debug) {
        if (sase_symbolic) {
          do_addi();
          sase_addi();
        }
      } else
        do_addi();

      return;
    }
  } else if (opcode == OP_LD) {
    decode_i_format();

    if (funct3 == F3_LD) {
      if (debug) {
        if (sase_symbolic) {
          sase_ld();
        }
      } else
        do_ld();

      return;
    }
  } else if (opcode == OP_SD) {
    decode_s_format();

    if (funct3 == F3_SD) {
      if (debug) {
        if (sase_symbolic) {
          sase_sd();
        }
      } else
        do_sd();

      return;
    }
  } else if (opcode == OP_OP) { // could be ADD, SUB, MUL, DIVU, REMU, SLTU
    decode_r_format();

    if (funct3 == F3_ADD) { // = F3_SUB = F3_MUL
      if (funct7 == F7_ADD) {
        if (debug) {
          if (sase_symbolic) {
            do_add();
            sase_add();
          }
        } else
          do_add();

        return;
      } else if (funct7 == F7_SUB) {
        if (debug) {
          if (sase_symbolic) {
            do_sub();
            sase_sub();
          }
        } else
          do_sub();

        return;
      } else if (funct7 == F7_MUL) {
        if (debug) {
          if (sase_symbolic) {
            do_mul();
            sase_mul();
          }
        } else
          do_mul();

        return;
      }
    } else if (funct3 == F3_DIVU) {
      if (funct7 == F7_DIVU) {
        if (debug) {
          if (sase_symbolic) {
            do_divu();
            sase_divu();
          }
        } else
          do_divu();

        return;
      }
    } else if (funct3 == F3_REMU) {
      if (funct7 == F7_REMU) {
        if (debug) {
          if (sase_symbolic) {
            do_remu();
            sase_remu();
          }
        } else
          do_remu();

        return;
      }
    } else if (funct3 == F3_SLTU) {
      if (funct7 == F7_SLTU) {
        if (debug) {
          if (sase_symbolic) {
            sase_sltu();
          }
        } else
          do_sltu();

        return;
      }
    }
  } else if (opcode == OP_BRANCH) {
    decode_b_format();

    if (funct3 == F3_BEQ) {
      if (debug) {
        if (sase_symbolic)
          do_beq();
      } else
        do_beq();

      return;
    }
  } else if (opcode == OP_JAL) {
    decode_j_format();

    if (debug) {
      if (sase_symbolic) {
        do_jal();
        sase_jal_jalr();
      }
    } else
      do_jal();

    return;
  } else if (opcode == OP_JALR) {
    decode_i_format();

    if (funct3 == F3_JALR) {
      if (debug) {
        if (sase_symbolic) {
          do_jalr();
          sase_jal_jalr();
        }
      } else
        do_jalr();

      return;
    }
  } else if (opcode == OP_LUI) {
    decode_u_format();

    if (debug) {
      if (sase_symbolic) {
        do_lui();
        sase_lui();
      }
    } else
      do_lui();

    return;
  } else if (opcode == OP_SYSTEM) {
    decode_i_format();

    if (funct3 == F3_ECALL) {
      if (debug) {
        if (sase_symbolic)
          do_ecall();
      } else
        do_ecall();

      return;
    }
  }

  if (execute)
    throw_exception(EXCEPTION_UNKNOWNINSTRUCTION, 0);
  else {
    //report the error on the console
    output_fd = 1;

    printf2((uint64_t*) "%s: unknown instruction with %x opcode detected\n", exe_name, (uint64_t*) opcode);

    exit(EXITCODE_UNKNOWNINSTRUCTION);
  }
}

void interrupt() {
  if (timer != TIMEROFF) {
    timer = timer - 1;

    if (timer == 0) {
      if (get_exception(current_context) == EXCEPTION_NOEXCEPTION)
        // only throw exception if no other is pending
        // TODO: handle multiple pending exceptions
        throw_exception(EXCEPTION_TIMER, 0);
      else
        // trigger timer in the next interrupt cycle
        timer = 1;
    }
  }
}

uint64_t* run_until_exception() {
  trap = 0;

  while (trap == 0) {
    fetch();
    decode_execute();
    interrupt();
  }

  trap = 0;

  return current_context;
}

uint64_t instruction_with_max_counter(uint64_t* counters, uint64_t max) {
  uint64_t a;
  uint64_t n;
  uint64_t i;
  uint64_t c;

  a = -1;
  n = 0;
  i = 0;

  while (i < code_length / INSTRUCTIONSIZE) {
    c = *(counters + i);

    if (n < c) {
      if (c < max) {
        n = c;
        a = i;
      } else
        return i * INSTRUCTIONSIZE;
    }

    i = i + 1;
  }

  if (a != -1)
    return a * INSTRUCTIONSIZE;
  else
    return -1;
}

uint64_t print_per_instruction_counter(uint64_t total, uint64_t* counters, uint64_t max) {
  uint64_t a;
  uint64_t c;

  a = instruction_with_max_counter(counters, max);

  if (a != -1) {
    c = *(counters + a / INSTRUCTIONSIZE);

    // CAUTION: we reset counter to avoid reporting it again
    *(counters + a / INSTRUCTIONSIZE) = 0;

    printf3((uint64_t*) ",%d(%.2d%%)@%x", (uint64_t*) c, (uint64_t*) fixed_point_percentage(fixed_point_ratio(total, c, 4), 4), (uint64_t*) a);
    print_code_line_number_for_instruction(a);

    return c;
  } else {
    print((uint64_t*) ",0(0.00%)");

    return 0;
  }
}

void print_per_instruction_profile(uint64_t* message, uint64_t total, uint64_t* counters) {
  printf3((uint64_t*) "%s%s%d", exe_name, message, (uint64_t*) total);
  print_per_instruction_counter(total, counters, print_per_instruction_counter(total, counters, print_per_instruction_counter(total, counters, UINT64_MAX_T)));
  println();
}

void print_profile() {
  printf4((uint64_t*)
    "%s: summary: %d executed instructions and %.2dMB(%.2d%%) mapped memory\n",
    exe_name,
    (uint64_t*) get_total_number_of_instructions(),
    (uint64_t*) fixed_point_ratio(pused(), MEGABYTE, 2),
    (uint64_t*) fixed_point_percentage(fixed_point_ratio(page_frame_memory, pused(), 4), 4));

  if (get_total_number_of_instructions() > 0) {
    print_instruction_counters();

    if (code_line_number != (uint64_t*) 0)
      printf1((uint64_t*) "%s: profile: total,max(ratio%%)@addr(line#),2max,3max\n", exe_name);
    else
      printf1((uint64_t*) "%s: profile: total,max(ratio%%)@addr,2max,3max\n", exe_name);

    print_per_instruction_profile((uint64_t*) ": calls:   ", calls, calls_per_procedure);
    print_per_instruction_profile((uint64_t*) ": loops:   ", iterations, iterations_per_loop);
    print_per_instruction_profile((uint64_t*) ": loads:   ", ic_ld, loads_per_instruction);
    print_per_instruction_profile((uint64_t*) ": stores:  ", ic_sd, stores_per_instruction);
  }
}

// -----------------------------------------------------------------
// ---------------------------- CONTEXTS ---------------------------
// -----------------------------------------------------------------

uint64_t* allocate_context(uint64_t* parent, uint64_t* vctxt, uint64_t* in) {
  uint64_t* context;

  if (free_contexts == (uint64_t*) 0)
    context = smalloc(7 * SIZEOFUINT64STAR + 9 * SIZEOFUINT64);
  else {
    context = free_contexts;

    free_contexts = get_next_context(free_contexts);
  }

  set_next_context(context, in);
  set_prev_context(context, (uint64_t*) 0);

  if (in != (uint64_t*) 0)
    set_prev_context(in, context);

  set_pc(context, 0);

  // allocate zeroed memory for general purpose registers
  // TODO: reuse memory
  set_regs(context, zalloc(NUMBEROFREGISTERS * REGISTERSIZE));

  // allocate zeroed memory for page table
  // TODO: save and reuse memory for page table
  set_pt(context, zalloc(VIRTUALMEMORYSIZE / PAGESIZE * REGISTERSIZE));

  // determine range of recently mapped pages
  set_lo_page(context, 0);
  set_me_page(context, 0);
  set_hi_page(context, get_page_of_virtual_address(VIRTUALMEMORYSIZE - REGISTERSIZE));

  set_exception(context, EXCEPTION_NOEXCEPTION);
  set_faulting_page(context, 0);

  set_exit_code(context, EXITCODE_NOERROR);

  set_parent(context, parent);
  set_virtual_context(context, vctxt);

  set_name(context, (uint64_t*) 0);

  return context;
}

void free_context(uint64_t* context) {
  set_next_context(context, free_contexts);

  free_contexts = context;
}

uint64_t* delete_context(uint64_t* context, uint64_t* from) {
  if (get_next_context(context) != (uint64_t*) 0)
    set_prev_context(get_next_context(context), get_prev_context(context));

  if (get_prev_context(context) != (uint64_t*) 0) {
    set_next_context(get_prev_context(context), get_next_context(context));
    set_prev_context(context, (uint64_t*) 0);
  } else
    from = get_next_context(context);

  free_context(context);

  return from;
}

// -----------------------------------------------------------------
// -------------------------- MICROKERNEL --------------------------
// -----------------------------------------------------------------

uint64_t* create_context(uint64_t* parent, uint64_t* vctxt) {
  // TODO: check if context already exists
  used_contexts = allocate_context(parent, vctxt, used_contexts);

  if (current_context == (uint64_t*) 0)
    current_context = used_contexts;

  if (debug_create)
    printf3((uint64_t*) "%s: parent context %p created child context %p\n", exe_name, parent, used_contexts);

  return used_contexts;
}

void map_page(uint64_t* context, uint64_t page, uint64_t frame) {
  uint64_t* table;

  table = get_pt(context);

  // assert: 0 <= page < VIRTUALMEMORYSIZE / PAGESIZE

  *(table + page) = frame;

  if (page <= get_page_of_virtual_address(get_program_break(context) - REGISTERSIZE)) {
    // exploit spatial locality in page table caching
    if (page < get_lo_page(context))
      set_lo_page(context, page);
    else if (page > get_me_page(context))
      set_me_page(context, page);
  }

  if (debug_map) {
    printf1((uint64_t*) "%s: page ", exe_name);
    print_hexadecimal(page, 4);
    printf2((uint64_t*) " mapped to frame %p in context %p\n", (uint64_t*) frame, context);
  }
}

// -----------------------------------------------------------------
// ---------------------------- KERNEL -----------------------------
// -----------------------------------------------------------------

uint64_t pavailable() {
  if (free_page_frame_memory > 0)
    return 1;
  else if (allocated_page_frame_memory + MEGABYTE <= page_frame_memory)
    return 1;
  else
    return 0;
}

uint64_t pexcess() {
  if (pavailable())
    return 1;
  else if (allocated_page_frame_memory + MEGABYTE <= 2 * page_frame_memory)
    // tolerate twice as much memory mapped on demand than physically available
    return 1;
  else
    return 0;
}

uint64_t pused() {
  return allocated_page_frame_memory - free_page_frame_memory;
}

uint64_t* palloc() {
  uint64_t block;
  uint64_t frame;

  // assert: page_frame_memory is equal to or a multiple of MEGABYTE
  // assert: PAGESIZE is a factor of MEGABYTE strictly less than MEGABYTE

  if (free_page_frame_memory == 0) {
    if (pexcess()) {
      free_page_frame_memory = MEGABYTE;

      // on boot level zero allocate zeroed memory
      block = (uint64_t) zalloc(free_page_frame_memory);

      allocated_page_frame_memory = allocated_page_frame_memory + free_page_frame_memory;

      // page frames must be page-aligned to work as page table index
      next_page_frame = round_up(block, PAGESIZE);

      if (next_page_frame > block)
        // losing one page frame to fragmentation
        free_page_frame_memory = free_page_frame_memory - PAGESIZE;
    } else {
      print(exe_name);
      print((uint64_t*) ": palloc out of physical memory\n");

      exit(EXITCODE_OUTOFPHYSICALMEMORY);
    }
  }

  frame = next_page_frame;

  next_page_frame = next_page_frame + PAGESIZE;

  free_page_frame_memory = free_page_frame_memory - PAGESIZE;

  // strictly, touching is only necessary on boot levels higher than zero
  return touch((uint64_t*) frame, PAGESIZE);
}

void pfree(uint64_t* frame) {
  // TODO: implement free list of page frames
}

void map_and_store(uint64_t* context, uint64_t vaddr, uint64_t data) {
  // assert: is_valid_virtual_address(vaddr) == 1

  if (is_virtual_address_mapped(get_pt(context), vaddr) == 0)
    map_page(context, get_page_of_virtual_address(vaddr), (uint64_t) palloc());

  if (sase_symbolic) {
    if (data < two_to_the_power_of_32)
      sase_store_memory(get_pt(context), vaddr, CONCRETE_T, data, boolector_unsigned_int(btor, data, bv_sort));
    else
      sase_store_memory(get_pt(context), vaddr, CONCRETE_T, data, boolector_unsigned_int_64(data));
  } else
    store_virtual_memory(get_pt(context), vaddr, data);
}

void up_load_binary(uint64_t* context) {
  uint64_t baddr;

  // assert: entry_point is multiple of PAGESIZE and REGISTERSIZE

  set_pc(context, entry_point);
  set_lo_page(context, get_page_of_virtual_address(entry_point));
  set_me_page(context, get_page_of_virtual_address(entry_point));
  set_original_break(context, entry_point + binary_length);
  set_program_break(context, get_original_break(context));

  baddr = 0;

  if (sase_symbolic) {
    // code is never constrained...
    sase_symbolic = 0;

    while (baddr < code_length) {
      map_and_store(context, entry_point + baddr, load_data(baddr));

      baddr = baddr + REGISTERSIZE;
    }

    // ... but data is
    sase_symbolic = 1;
  }

  while (baddr < binary_length) {
    map_and_store(context, entry_point + baddr, load_data(baddr));

    baddr = baddr + REGISTERSIZE;
  }

  set_name(context, binary_name);
}

uint64_t up_load_string(uint64_t* context, uint64_t* s, uint64_t SP) {
  uint64_t bytes;
  uint64_t i;

  bytes = round_up(string_length(s) + 1, REGISTERSIZE);

  // allocate memory for storing string
  SP = SP - bytes;

  i = 0;

  while (i < bytes) {
    map_and_store(context, SP + i, *s);

    s = s + 1;

    i = i + REGISTERSIZE;
  }

  return SP;
}

void up_load_arguments(uint64_t* context, uint64_t argc, uint64_t* argv) {
  /* upload arguments like a UNIX system

      SP
      |
      V
   | argc | argv[0] | ... | argv[n] | 0 | env[0] | ... | env[m] | 0 |

     with argc > 0, n == argc - 1, and m == 0 (that is, env is empty) */
  uint64_t SP;
  uint64_t* vargv;
  uint64_t i;

  // the call stack grows top down
  SP = VIRTUALMEMORYSIZE;

  vargv = smalloc(argc * SIZEOFUINT64STAR);

  i = 0;

  // push program parameters onto the stack
  while (i < argc) {
    SP = up_load_string(context, (uint64_t*) *(argv + i), SP);

    // store pointer in virtual *argv
    *(vargv + i) = SP;

    i = i + 1;
  }

  // allocate memory for termination of env table
  SP = SP - REGISTERSIZE;

  // push null value to terminate env table
  map_and_store(context, SP, 0);

  // allocate memory for termination of argv table
  SP = SP - REGISTERSIZE;

  // push null value to terminate argv table
  map_and_store(context, SP, 0);

  // assert: i == argc

  // push argv table onto the stack
  while (i > 0) {
    // allocate memory for argv table entry
    SP = SP - REGISTERSIZE;

    i = i - 1;

    // push argv table entry
    map_and_store(context, SP, *(vargv + i));
  }

  // allocate memory for argc
  SP = SP - REGISTERSIZE;

  // push argc
  map_and_store(context, SP, argc);

  // store stack pointer value in stack pointer register
  *(get_regs(context) + REG_SP) = SP;

  // set bounds to register value for symbolic execution
  if (sase_symbolic) {
    // assert: SP < 2^32
    sase_regs[REG_SP] = boolector_unsigned_int(btor, SP, bv_sort);
    sase_regs_typ[REG_SP] = CONCRETE_T;
  }
}

uint64_t handle_system_call(uint64_t* context) {
  uint64_t a7;

  set_exception(context, EXCEPTION_NOEXCEPTION);

  a7 = *(get_regs(context) + REG_A7);

  if (a7 == SYSCALL_BRK)
    implement_brk(context);
  else if (a7 == SYSCALL_READ)
    implement_read(context);
  else if (a7 == SYSCALL_WRITE)
    implement_write(context);
  else if (a7 == SYSCALL_OPEN)
    implement_open(context);
  else if (a7 == SYSCALL_SYMPOLIC_INPUT)
    implement_symbolic_input(context);
  else if (a7 == SYSCALL_EXIT) {
    implement_exit(context);

    // TODO: exit only if all contexts have exited
    return EXIT;
  } else if (a7 == SYSCALL_ASSERT_ZONE_BGN)
    implement_assert_begin(context);
  else if (a7 == SYSCALL_ASSERT)
    implement_assert(context);
  else if (a7 == SYSCALL_ASSERT_ZONE_END)
    implement_assert_end(context);
  else {
    printf2((uint64_t*) "%s: unknown system call %d\n", exe_name, (uint64_t*) a7);

    set_exit_code(context, EXITCODE_UNKNOWNSYSCALL);

    return EXIT;
  }

  return DONOTEXIT;
}

uint64_t handle_page_fault(uint64_t* context) {
  set_exception(context, EXCEPTION_NOEXCEPTION);

  // TODO: use this table to unmap and reuse frames
  map_page(context, get_faulting_page(context), (uint64_t) palloc());

  return DONOTEXIT;
}

uint64_t handle_division_by_zero(uint64_t* context) {
  set_exception(context, EXCEPTION_NOEXCEPTION);

  // printf1((uint64_t*) "%s: division by zero\n", exe_name);

  set_exit_code(context, EXITCODE_DIVISIONBYZERO);

  return EXIT;
}

uint64_t is_max_trace_reached = 0;

uint64_t handle_max_trace(uint64_t* context) {
  set_exception(context, EXCEPTION_NOEXCEPTION);

  set_exit_code(context, EXITCODE_OUTOFTRACEMEMORY);

  if (is_max_trace_reached == 0) {
    printf("%s\n", "max trace length reached, continue the execution");
    is_max_trace_reached = 1;
  }

  return EXIT;
}

uint64_t handle_timer(uint64_t* context) {
  set_exception(context, EXCEPTION_NOEXCEPTION);

  return DONOTEXIT;
}

uint64_t handle_exception(uint64_t* context) {
  uint64_t exception;

  exception = get_exception(context);

  if (exception == EXCEPTION_SYSCALL)
    return handle_system_call(context);
  else if (exception == EXCEPTION_PAGEFAULT)
    return handle_page_fault(context);
  else if (exception == EXCEPTION_DIVISIONBYZERO)
    return handle_division_by_zero(context);
  else if (exception == EXCEPTION_MAXTRACE)
    return handle_max_trace(context);
  else if (exception == EXCEPTION_TIMER)
    return handle_timer(context);
  else {
    printf2((uint64_t*) "%s: context %s throws uncaught ", exe_name, get_name(context));
    print_exception(exception, get_faulting_page(context));
    println();

    set_exit_code(context, EXITCODE_UNCAUGHTEXCEPTION);

    return EXIT;
  }
}

uint64_t engine(uint64_t* to_context) {
  registers = get_regs(to_context);
  pt        = get_pt(to_context);

  while (1) {
    // restore machine state
    pc = get_pc(current_context);

    run_until_exception();

    // save machine state
    set_pc(current_context, pc);

    if (handle_exception(current_context) == EXIT) {

      if (sase_symbolic) {
        if (sase_tc == 0 || pc == 0) {
          printf(GREEN "backtracking: %llu\n" RESET, ++b);
          return EXITCODE_NOERROR;
        } else {
          b++;

          sase_backtrack_sltu(0);
          set_pc(current_context, pc);

          if (pc == 0) {
            printf(GREEN "backtracking: %llu\n" RESET, b);
            return EXITCODE_NOERROR;
          }
        }
      }

    }

  }
}

uint64_t selfie_run(uint64_t machine) {
  uint64_t exit_code;

  if (binary_length == 0) {
    printf("%s\n", "nothing to run");

    return EXITCODE_BADARGUMENTS;
  }

  debug         = 1;
  sase_symbolic = 1;

  init_sase();

  init_memory(round_up(4 * sase_trace_size * SIZEOFUINT64, MEGABYTE) / MEGABYTE + 1);

  fuzz = atoi(peek_argument());

  execute = 1;

  reset_interpreter();
  reset_microkernel();

  create_context(MY_CONTEXT, 0);

  up_load_binary(current_context);

  // pass binary name as first argument by replacing memory size
  set_argument(binary_name);

  up_load_arguments(current_context, number_of_remaining_arguments(), remaining_arguments());

  printf3((uint64_t*) "%s: phantom executing %s with %dMB physical memory \n", exe_name, binary_name, (uint64_t*) (page_frame_memory / MEGABYTE));
  printf("\n");

  exit_code = engine(current_context);

  execute = 0;

  printf("\n");
  printf3((uint64_t*) "%s: phantom terminating %s with exit code %d\n", exe_name, get_name(current_context), (uint64_t*) sign_extend(exit_code, SYSCALL_BITWIDTH));

  print_profile();

  sase_symbolic = 0;
  record        = 0;
  disassemble   = 0;
  debug         = 0;

  fuzz = 0;

  return exit_code;
}

// -----------------------------------------------------------------
// ----------------------------- MAIN ------------------------------
// -----------------------------------------------------------------

uint64_t number_of_remaining_arguments() {
  return selfie_argc;
}

uint64_t* remaining_arguments() {
  return selfie_argv;
}

uint64_t* peek_argument() {
  if (number_of_remaining_arguments() > 0)
    return (uint64_t*) *selfie_argv;
  else
    return (uint64_t*) 0;
}

uint64_t* get_argument() {
  uint64_t* argument;

  argument = peek_argument();

  if (number_of_remaining_arguments() > 0) {
    selfie_argc = selfie_argc - 1;
    selfie_argv = selfie_argv + 1;
  }

  return argument;
}

void set_argument(uint64_t* argv) {
  *selfie_argv = (uint64_t) argv;
}

void print_usage() {
  printf("usage: executable -l binary -sase fuzz \n");
}

uint64_t main(uint64_t argc, uint64_t* argv) {
  uint64_t* option;

  init_selfie((uint64_t) argc, (uint64_t*) argv);

  if (number_of_remaining_arguments() < 4) {
    print_usage();
  } else {
    init_library();
    init_register();
    init_interpreter();

    option = get_argument();
    if (string_compare(option, (uint64_t*) "-l")) {
      selfie_load();
    } else {
      print_usage();
      return EXITCODE_BADARGUMENTS;
    }

    option = get_argument();
    if (string_compare(option, (uint64_t*) "-sase")) {
      return selfie_run(SASE);
    } else {
      print_usage();
      return EXITCODE_BADARGUMENTS;
    }

  }

  return 0;
}