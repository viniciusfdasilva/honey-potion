// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:472
// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:41
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:52
#ifndef __inline
#define __inline \
  inline __attribute__((always_inline))
#endif

#define MAX_ITERATION 100
#define MAX_STR_SIZE 50
#define STRING_POOL_SIZE 500
#define TUPLE_POOL_SIZE 500
#define HEAP_SIZE 100
#define MAX_STRUCT_MEMBERS 5

#define field_pad 1
#define field_syscall_nr 2
#define field_pid 3
#define field_sig 4

#define BINARY_OPERATION(generic_result, op, var1, var2) \
  op(&op_result, &var1, &var2);                          \
  if (op_result.exception)                               \
    goto CATCH;                                          \
  Generic generic_result;                                \
  generic_result.type = op_result.result_var.type;       \
  generic_result.value = op_result.result_var.value;

#define ATOM_NIL                               \
  (Generic)                                    \
  {                                            \
    .type = ATOM, .value.string = {.start = 0, \
                                   .end = 2 }  \
  }
#define ATOM_FALSE                             \
  (Generic)                                    \
  {                                            \
    .type = ATOM, .value.string = {.start = 3, \
                                   .end = 7 }  \
  }
#define ATOM_TRUE                              \
  (Generic)                                    \
  {                                            \
    .type = ATOM, .value.string = {.start = 8, \
                                   .end = 11 } \
  }

#define INT_MAX 2147483647
#define INT_MIN -2147483648

#define QUOTE_HELPER(expr) #expr
#define QUOTE(expr) QUOTE_HELPER(expr)
// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:91
typedef struct Generic Generic;
typedef enum Operation Operation;
typedef enum Type Type;
typedef struct Tuple Tuple;
typedef union ElixirValue ElixirValue;

typedef enum Type
{
  INVALID_TYPE,
  PATTERN_M,
  INTEGER,
  DOUBLE,
  STRING,
  ATOM,
  TUPLE,
  LIST,
  STRUCT,
  TYPE_Syscalls_enter_kill_arg
} Type;

typedef struct Tuple
{
  int start;
  int end;
} Tuple;

typedef struct String
{
  int start;
  int end;
} String;

typedef struct StrToPrint
{
  char str[MAX_STR_SIZE + 6];
} StrToPrint;

typedef struct struct_Syscalls_enter_kill_args
{
  unsigned pos_pad;
  unsigned pos_syscall_nr;
  unsigned pos_pid;
  unsigned pos_sig;
} struct_Syscalls_enter_kill_args;

typedef union ElixirValue
{
  int integer;
  unsigned u_integer;
  double double_precision;
  Tuple tuple;
  String string;
  struct_Syscalls_enter_kill_args syscalls_enter_kill_args;
} ElixirValue;

typedef struct Generic
{
  Type type;
  ElixirValue value;
} Generic;

typedef struct OpResult
{
  Generic result_var;
  int exception;
  char exception_msg[150];
} OpResult;

typedef struct StrFormatSpec
{
  char spec[2];
} StrFormatSpec;

struct syscalls_enter_kill_args
{
  /**
   * This is the tracepoint arguments of the kill functions.
   * Defined at: /sys/kernel/debug/tracing/events/syscalls/sys_enter_kill/format
   */
  long long pad;

  long syscall_nr;
  long pid;
  long sig;
};

// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:182
// String pool
struct
{
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, 1);
  __uint(key_size, sizeof(int));
  __uint(value_size, sizeof(char[STRING_POOL_SIZE]));
} string_pool_map SEC(".maps");

struct
{
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, 1);
  __uint(key_size, sizeof(int));
  __uint(value_size, sizeof(int));
} string_pool_index_map SEC(".maps");

// Tuple
struct
{
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, 1);
  __uint(key_size, sizeof(int));
  __uint(value_size, sizeof(unsigned[TUPLE_POOL_SIZE]));
} tuple_pool_map SEC(".maps");

struct
{
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, 1);
  __uint(key_size, sizeof(int));
  __uint(value_size, sizeof(unsigned));
} tuple_pool_index_map SEC(".maps");

// Heap
struct
{
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, 1);
  __uint(key_size, sizeof(int));
  __uint(value_size, sizeof(Generic[HEAP_SIZE]));
} heap_map SEC(".maps");

struct
{
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __uint(max_entries, 1);
  __uint(key_size, sizeof(int));
  __uint(value_size, sizeof(int));
} heap_index_map SEC(".maps");

// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:440
char LICENSE[] SEC("license") = "Dual BSD/GPL";

static int to_bool(Generic *var)
{
  if (var->type == ATOM)
  {
    int zero = 0;
    unsigned start = var->value.string.start;
    unsigned end = var->value.string.end;
    char(*string_pool)[STRING_POOL_SIZE] = bpf_map_lookup_elem(&string_pool_map, &zero);
    if (!string_pool)
    {
      bpf_printk("(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool, to_bool function).");
      return 0;
    }

    /*
      nil = {start: 0, end: 2}
      false = {start: 3, end: 7}
      true = {start: 8, end: 11}
    */

    int str_size = end - start + 1;
    if (str_size == 3)
    {
      if (start + 3 >= STRING_POOL_SIZE)
      {
        return 0;
      }

      if ((*string_pool)[start] == 'n' &&
          (*string_pool)[start + 1] == 'i' &&
          (*string_pool)[start + 2] == 'l')
      {
        return 0;
      }
    }

    else if (str_size == 5)
    {
      if (start + 5 >= STRING_POOL_SIZE)
      {
        return 0;
      }

      if ((*string_pool)[start] == 'f' &&
          (*string_pool)[start + 1] == 'a' &&
          (*string_pool)[start + 2] == 'l' &&
          (*string_pool)[start + 3] == 's' &&
          (*string_pool)[start + 4] == 'e')
      {
        return 0;
      }
    }
  }

  return 1;
}

static int values_are_equal(Generic *var1, Generic *var2)
{
  if (var1->type == INTEGER)
  {
    if (var2->type == INTEGER)
    {
      if (var1->value.integer == var2->value.integer)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }

  return 0;
}

static char get_str_format_specifier(Generic *g, StrFormatSpec *result)
{
  // TODO
  if (g->type == INTEGER)
  {
    *result = (StrFormatSpec){"%d"};
  }
}

// Unary operations
static void negate(Generic *var, Generic *result)
{
  // TODO
}

// Binary operations
static void Subtract(OpResult *result, Generic *var1, Generic *var2)
{
  result->exception = 0;

  if (var1->type == DOUBLE || var2->type == DOUBLE)
  {
    *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (subtraction with floats)."};
    return;
  }

  if (var1->type == INTEGER)
  {
    if (var2->type == INTEGER)
    {
      // Overflow:
      if ((var1->value.integer < 0) && (var2->value.integer > INT_MAX + var1->value.integer))
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (Subtraction of two values is greater than " QUOTE(INT_MAX) ")."};
        return;
      }
      // Underflow:
      if ((var1->value.integer > 0) && (var2->value.integer < INT_MIN + var1->value.integer))
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (Subtraction of two values is smaller than " QUOTE(INT_MIN) ")."};
        return;
      }

      result->result_var.type = INTEGER;
      result->result_var.value.integer = var1->value.integer + var2->value.integer;
      return;
    }
  }

  *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (function Sum)."};
}

static void Sum(OpResult *result, Generic *var1, Generic *var2)
{
  result->exception = 0;

  if (var1->type == DOUBLE || var2->type == DOUBLE)
  {
    *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (sum with floats)."};
    return;
  }

  if (var1->type == INTEGER)
  {
    if (var2->type == INTEGER)
    {
      // Overflow:
      if ((var1->value.integer > 0) && (var2->value.integer > INT_MAX - var1->value.integer))
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (Sum of two values is greater than " QUOTE(INT_MAX) ")."};
        return;
      }
      // Underflow:
      if ((var1->value.integer < 0) && (var2->value.integer < INT_MIN - var1->value.integer))
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (Sum of two values is smaller than " QUOTE(INT_MIN) ")."};
        return;
      }

      result->result_var.type = INTEGER;
      result->result_var.value.integer = var1->value.integer + var2->value.integer;
      return;
    }
  }

  *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (function Sum)."};
}

static void Divide(OpResult *result, Generic *var1, Generic *var2)
{
  result->exception = 0;
  if (var1->type == DOUBLE || var2->type == DOUBLE)
  {
    *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (division with float)."};
    return;
  }

  if (var1->type == INTEGER)
  {
    if (var2->type == INTEGER)
    {
      if (var2->value.integer == 0)
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (division by zero)."};
        return;
      }

      // Overflow
      if (var1->value.integer == INT_MIN && var2->value.integer == -1)
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (division of " QUOTE(INT_MIN) " by -1 exceeded the limit of " QUOTE(INT_MAX) ")"};
        return;
      }

      unsigned u_var1 = var1->value.integer < 0 ? var1->value.integer * -1 : var1->value.integer;
      unsigned u_var2 = var2->value.integer < 0 ? var2->value.integer * -1 : var2->value.integer;
      if ((var1->value.integer <= 0 && var2->value.integer < 0) || (var1->value.integer >= 0 && var2->value.integer > 0))
      {
        result->result_var.type = INTEGER;
        result->result_var.value.integer = u_var1 / u_var2;
        return;
      }
      else
      {
        result->result_var.type = INTEGER;
        result->result_var.value.integer = (u_var1 / u_var2) * -1;
        return;
      }
    }
  }

  *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (function Divide)."};
}

static void Multiply(OpResult *result, Generic *var1, Generic *var2)
{
  result->exception = 0;
  if (var1->type == DOUBLE || var2->type == DOUBLE)
  {
    *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (multiplication with float)."};
    return;
  }

  if (var1->type == INTEGER)
  {
    if (var2->type == INTEGER)
    {
      if (var1->value.integer == 0 || var2->value.integer == 0)
      {
        result->result_var.type = INTEGER;
        result->result_var.value.integer = 0;
        return;
      }

      if ((var1->value.integer == INT_MIN && var2->value.integer != 1) || (var2->value.integer == INT_MIN && var1->value.integer != 1))
      {
        *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (multiplication of two numbers exceeded integer boundaries.)"};
        return;
      }

      unsigned u_var1 = var1->value.integer < 0 ? var1->value.integer * -1 : var1->value.integer;
      unsigned u_var2 = var2->value.integer < 0 ? var2->value.integer * -1 : var2->value.integer;
      // Overflow
      if ((var1->value.integer > 0 && var2->value.integer > 0) || (var1->value.integer < 0 && var2->value.integer < 0))
      {
        if (u_var1 > INT_MAX / u_var2)
        {
          *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (multiplication of two values exceeded " QUOTE(INT_MAX) ")."};
          return;
        }
      }
      else
      {
        // Underflow
        unsigned int_min_positive = INT_MIN * -1;
        if (u_var1 > int_min_positive / u_var2)
        {
          *result = (OpResult){.exception = 1, .exception_msg = "(ArithmeticError) bad argument in arithmetic expression (multiplication of two values receded " QUOTE(INT_MIN) ")."};
          return;
        }
      }

      result->result_var.type = INTEGER;
      result->result_var.value.integer = var1->value.integer * var2->value.integer;
      return;
    }
  }

  *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (function Multiply)."};
}

static void Copy(OpResult *result, Generic *to, Generic *from)
{
  if (from->type == STRING)
  {
    to->type = STRING;

    int zero = 0;
    int *string_pool_index = bpf_map_lookup_elem(&string_pool_index_map, &zero);
    if (!string_pool_index)
    {
      *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool index, function Copy)."};
      return;
    }

    int str_len = from->value.string.end - from->value.string.start + 1;
    to->value.string.start = *string_pool_index;
    to->value.string.end = to->value.string.start + str_len - 1;

    if (to->value.string.end > STRING_POOL_SIZE - 1)
    {
      *result = (OpResult){.exception = 1, .exception_msg = "(StringPoolSizeError) The program reached the maximum size of " QUOTE(STRING_POOL_SIZE) " characters stored in all bitstrings."};
    }

    char(*string_pool)[STRING_POOL_SIZE] = bpf_map_lookup_elem(&string_pool_map, &zero);
    if (!string_pool)
    {
      *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool, function Copy)."};
      return;
    }

    __builtin_memcpy(*string_pool[to->value.string.start], *string_pool[from->value.string.start], str_len);
  }
}

static int pattern_match(Generic *var1, Generic *var2)
{
  // TODO
  return 0;
}

static void Equals(OpResult *result, Generic *var1, Generic *var2)
{
  result->exception = 0;

  if (var1->type != var2->type)
  {
    result->result_var = ATOM_FALSE;
    return;
  }

  if (var1->type == INTEGER)
  {
    if (var1->value.integer == var2->value.integer)
    {
      result->result_var = ATOM_TRUE;
    }
    else
    {
      result->result_var = ATOM_FALSE;
    }

    return;
  }

  *result = (OpResult){
      .exception = 1,
      .exception_msg =
          "(AplhaVersion) Currently, we can only compare integers with the '==' operator."};
  return;
} // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:275
static void getMember(OpResult *result, Generic *elixir_struct, char member_name[20], Generic *member)
{
  *result = (OpResult){.exception = 0};
  int zero = 0;
  Generic(*heap)[HEAP_SIZE] = bpf_map_lookup_elem(&heap_map, &zero);
  if (!heap)
  {
    *result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool, getMember function)."};
    return;
  }
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:286
  if (elixir_struct->type == TYPE_Syscalls_enter_kill_arg)
  {
    if (__builtin_memcmp(member_name, "pad", 4) == 0)
    {
      unsigned index = elixir_struct->value.syscalls_enter_kill_args.pos_pad;
      if (index < HEAP_SIZE)
      {
        *member = (*heap)[index];
      }
    }
    else if (__builtin_memcmp(member_name, "syscall_nr", 11) == 0)
    {
      unsigned index = elixir_struct->value.syscalls_enter_kill_args.pos_syscall_nr;
      if (index < HEAP_SIZE)
      {
        *member = (*heap)[index];
      }
    }
    else if (__builtin_memcmp(member_name, "pid", 4) == 0)
    {
      unsigned index = elixir_struct->value.syscalls_enter_kill_args.pos_pid;
      if (index < HEAP_SIZE)
      {
        *member = (*heap)[index];
      }
    }
    else if (__builtin_memcmp(member_name, "sig", 4) == 0)
    {
      unsigned index = elixir_struct->value.syscalls_enter_kill_args.pos_sig;
      if (index < HEAP_SIZE)
      {
        *member = (*heap)[index];
      }
    }
  }
  *result = (OpResult){.exception = 1, .exception_msg = "(InvalidMember) Tried to access invalid member of a struct."};
  return;
}

// Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:458
SEC("tracepoint/syscalls/sys_enter_kill")
int main_func(struct syscalls_enter_kill_args *ctx_arg)
{
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:337
  StrFormatSpec str_param1;
  StrFormatSpec str_param2;
  StrFormatSpec str_param3;

  OpResult op_result;

  int zero = 0;
  char(*string_pool)[STRING_POOL_SIZE] = bpf_map_lookup_elem(&string_pool_map, &zero);
  if (!string_pool)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool, main function)."};
    goto CATCH;
  }

  unsigned *string_pool_index = bpf_map_lookup_elem(&string_pool_index_map, &zero);
  if (!string_pool_index)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool index, main function)."};
    goto CATCH;
  }
  *string_pool_index = 0;

  __builtin_memcpy(*string_pool, "nil", 3);
  __builtin_memcpy(*string_pool + 3, "false", 5);
  __builtin_memcpy(*string_pool + 3 + 5, "true", 4);

  Generic(*heap)[HEAP_SIZE] = bpf_map_lookup_elem(&heap_map, &zero);
  if (!heap)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access heap map, main function)."};
    goto CATCH;
  }

  unsigned *heap_index = bpf_map_lookup_elem(&heap_index_map, &zero);
  if (!heap_index)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access heap map index, main function)."};
    goto CATCH;
  }
  *heap_index = 0;

  unsigned(*tuple_pool)[TUPLE_POOL_SIZE] = bpf_map_lookup_elem(&tuple_pool_map, &zero);
  if (!tuple_pool)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool, main function)."};
    goto CATCH;
  }

  unsigned *tuple_pool_index = bpf_map_lookup_elem(&tuple_pool_index_map, &zero);
  if (!tuple_pool_index)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(UnexpectedBehavior) something wrong happened inside the Elixir runtime for eBPF. (can't access string pool index, main function)."};
    goto CATCH;
  }
  *tuple_pool_index = 0;

  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:402
  Generic ctx0nil = {.type = TYPE_Syscalls_enter_kill_arg, .value.syscalls_enter_kill_args = {(*heap_index)++, (*heap_index)++, (*heap_index)++, (*heap_index)++}};
  unsigned last_index = ctx0nil.value.syscalls_enter_kill_args.pos_sig;
  if (ctx0nil.value.syscalls_enter_kill_args.pos_pad < HEAP_SIZE)
  {
    // (*heap)[ctx0nil.value.syscalls_enter_kill_args.pos_pad] = (Generic){.type = INTEGER, .value.integer = ctx_arg->pad};
  }
  if (ctx0nil.value.syscalls_enter_kill_args.pos_syscall_nr < HEAP_SIZE)
  {
    (*heap)[ctx0nil.value.syscalls_enter_kill_args.pos_syscall_nr] = (Generic){.type = INTEGER, .value.integer = ctx_arg->syscall_nr};
  }
  if (ctx0nil.value.syscalls_enter_kill_args.pos_pid < HEAP_SIZE)
  {
    (*heap)[ctx0nil.value.syscalls_enter_kill_args.pos_pid] = (Generic){.type = INTEGER, .value.integer = ctx_arg->pid};
  }
  if (ctx0nil.value.syscalls_enter_kill_args.pos_sig < HEAP_SIZE)
  {
    (*heap)[ctx0nil.value.syscalls_enter_kill_args.pos_sig] = (Generic){.type = INTEGER, .value.integer = ctx_arg->sig};
  }

  // =============== beginning of user code ===============

  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:382
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:273
  Generic helper_var_227 = {.type = INTEGER, .value.integer = 1};
  Generic helper_var_259 = {.type = INTEGER, .value.integer = 2};
  if (*heap_index < (HEAP_SIZE - 0))
  {
    (*heap)[(*heap_index) + 0] = helper_var_227;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  if (*tuple_pool_index < (TUPLE_POOL_SIZE - 0))
  {
    (*tuple_pool)[(*tuple_pool_index) + 0] = (*heap_index) + 0;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to create tuple, the tuple pool is full."};
    goto CATCH;
  }
  if (*heap_index < (HEAP_SIZE - 1))
  {
    (*heap)[(*heap_index) + 1] = helper_var_259;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  if (*tuple_pool_index < (TUPLE_POOL_SIZE - 1))
  {
    (*tuple_pool)[(*tuple_pool_index) + 1] = (*heap_index) + 1;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to create tuple, the tuple pool is full."};
    goto CATCH;
  }
  *heap_index += 2;
  *tuple_pool_index += 2;

  Generic helper_var_291 = {.type = TUPLE, .value.tuple = (Tuple){.start = (*tuple_pool_index) - 2, .end = (*tuple_pool_index) - 1}};

  op_result.exception = 0;
  Generic one_two0 = helper_var_291;

label_98:
  if (op_result.exception == 1)
  {
    goto CATCH;
  }

  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:382
  Generic helper_var_162 = {.type = INTEGER, .value.integer = 3};
  op_result.exception = 0;
  Generic three1 = helper_var_162;

label_130:
  if (op_result.exception == 1)
  {
    goto CATCH;
  }

  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:382
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:334
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:334
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:289
  Generic helper_var_226 = {.type = LIST, .value.tuple = (Tuple){.start = -1, .end = -1}};
  if ((*heap_index) < HEAP_SIZE && (*heap_index) >= 0)
  {
    (*heap)[(*heap_index)] = helper_var_226;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  ++(*heap_index);
  if (*tuple_pool_index < TUPLE_POOL_SIZE && *tuple_pool_index >= 0)
  {
    (*tuple_pool)[(*tuple_pool_index)] = (*heap_index);
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to create tuple, the tuple pool is full."};
    goto CATCH;
  }
  ++(*tuple_pool_index);
  if (*tuple_pool_index < TUPLE_POOL_SIZE && *tuple_pool_index >= 0)
  {
    (*tuple_pool)[(*tuple_pool_index)] = (*heap_index) - 1;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to create tuple, the tuple pool is full."};
    goto CATCH;
  }
  ++(*tuple_pool_index);

  Generic helper_var_258 = (Generic){.type = LIST, .value.tuple = (Tuple){.start = (*tuple_pool_index) - 2, .end = (*tuple_pool_index) - 1}};
  if (*heap_index < HEAP_SIZE && *heap_index >= 0)
  {
    (*heap)[(*heap_index)] = one_two0;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  ++(*heap_index);
  if (*heap_index < HEAP_SIZE && *heap_index >= 0)
  {
    (*heap)[(*heap_index)] = helper_var_258;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  ++(*heap_index);
  if (*tuple_pool_index < TUPLE_POOL_SIZE && *tuple_pool_index >= 0)
  {
    (*tuple_pool)[(*tuple_pool_index)] = (*heap_index);
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to create tuple, the tuple pool is full."};
    goto CATCH;
  }
  ++(*tuple_pool_index);
  if (*tuple_pool_index < TUPLE_POOL_SIZE && *tuple_pool_index >= 0)
  {
    (*tuple_pool)[(*tuple_pool_index)] = (*heap_index) - 1;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to create tuple, the tuple pool is full."};
    goto CATCH;
  }
  ++(*tuple_pool_index);

  Generic helper_var_290 = (Generic){.type = LIST, .value.tuple = (Tuple){.start = (*tuple_pool_index) - 2, .end = (*tuple_pool_index) - 1}};
  if (*heap_index < HEAP_SIZE && *heap_index >= 0)
  {
    (*heap)[(*heap_index)] = three1;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  ++(*heap_index);
  if (*heap_index < HEAP_SIZE && *heap_index >= 0)
  {
    (*heap)[(*heap_index)] = helper_var_290;
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MemoryLimitReached) Impossible to allocate memory in the heap."};
    goto CATCH;
  }
  ++(*heap_index);

  op_result.exception = 0;
  if (helper_var_290.type != LIST)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
    goto label_194;
  }
  Generic helper_var_322;
  if (helper_var_290.value.tuple.start < TUPLE_POOL_SIZE && helper_var_290.value.tuple.start >= 0)
  {
    unsigned helper_var_386 = *((*tuple_pool) + (helper_var_290.value.tuple.start));
    if (helper_var_386 < HEAP_SIZE && helper_var_386 >= 0)
    {
      helper_var_322 = *(*(heap) + (helper_var_386));
    }
    else
    {
      op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
      goto label_194;
    }
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
    goto label_194;
  }

  Generic label_354;
  if (helper_var_290.value.tuple.start + 1 < TUPLE_POOL_SIZE && helper_var_290.value.tuple.start + 1 >= 0)
  {
    unsigned helper_var_386 = *((*tuple_pool) + (helper_var_290.value.tuple.start + 1));
    if (helper_var_386 < HEAP_SIZE && helper_var_386 >= 0)
    {
      label_354 = *(*(heap) + (helper_var_386));
    }
    else
    {
      op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
      goto label_194;
    }
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
    goto label_194;
  }
  Generic helper_var_418;
  if (label_354.value.tuple.start < TUPLE_POOL_SIZE && label_354.value.tuple.start >= 0)
  {
    unsigned helper_var_482 = *((*tuple_pool) + (label_354.value.tuple.start));
    if (helper_var_482 < HEAP_SIZE && helper_var_482 >= 0)
    {
      helper_var_418 = *(*(heap) + (helper_var_482));
    }
    else
    {
      op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
      goto label_194;
    }
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
    goto label_194;
  }

  Generic label_450;
  if (label_354.value.tuple.start + 1 < TUPLE_POOL_SIZE && label_354.value.tuple.start + 1 >= 0)
  {
    unsigned helper_var_482 = *((*tuple_pool) + (label_354.value.tuple.start + 1));
    if (helper_var_482 < HEAP_SIZE && helper_var_482 >= 0)
    {
      label_450 = *(*(heap) + (helper_var_482));
    }
    else
    {
      op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
      goto label_194;
    }
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
    goto label_194;
  }
  Generic any_tuple2 = helper_var_418;

label_194:
  if (op_result.exception == 1)
  {
    goto CATCH;
  }

  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:382

  op_result.exception = 0;
  if (any_tuple2.type != TUPLE)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(MatchError) No match of right hand side value."};
    goto label_514;
  }
  Generic helper_var_546;
  if (any_tuple2.value.tuple.start + 0 < TUPLE_POOL_SIZE && any_tuple2.value.tuple.start + 0 >= 0)
  {
    unsigned helper_var_578 = *((*tuple_pool) + (any_tuple2.value.tuple.start + 0));
    if (helper_var_578 < HEAP_SIZE && helper_var_578 >= 0)
    {
      helper_var_546 = *(*(heap) + (helper_var_578));
    }
    else
    {
      op_result = (OpResult){.exception = 1, .exception_msg = "HEAP(MatchError) No match of right hand side value."};
      goto label_514;
    }
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "TUPLE(MatchError) No match of right hand side value."};
    goto label_514;
  }
  Generic x3 = helper_var_546;
  Generic helper_var_610;
  if (any_tuple2.value.tuple.start + 1 < TUPLE_POOL_SIZE && any_tuple2.value.tuple.start + 1 >= 0)
  {
    unsigned helper_var_642 = *((*tuple_pool) + (any_tuple2.value.tuple.start + 1));
    if (helper_var_642 < HEAP_SIZE && helper_var_642 >= 0)
    {
      helper_var_610 = *(*(heap) + (helper_var_642));
    }
    else
    {
      op_result = (OpResult){.exception = 1, .exception_msg = "HEAP(MatchError) No match of right hand side value."};
      goto label_514;
    }
  }
  else
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "TUPLE(MatchError) No match of right hand side value."};
    goto label_514;
  }

label_514:
  if (op_result.exception == 1)
  {
    goto CATCH;
  }

  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/translator.ex:123

  bpf_printk("x: %d", x3.value.integer);
  Generic helper_var_674 = {.type = INTEGER, .value.integer = 0};

  // =============== end of user code ==============
  // Generated at /home/vrjuliao/workfolder/lac-dcc/honey-potion/lib/honey/boilerplates.ex:426
  if (helper_var_674.type != INTEGER)
  {
    op_result = (OpResult){.exception = 1, .exception_msg = "(IncorrectReturn) eBPF function is not returning an integer."};
    goto CATCH;
  }
  return helper_var_674.value.integer;

CATCH:
  bpf_printk("** %s\n", op_result.exception_msg);
  return 0;
}
