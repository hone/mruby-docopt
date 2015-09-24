typedef enum {
  STRING,
  BOOL,
  LONG,
  EMPTY,
  STRINGLIST
} type_enum;

const char * type_enum_to_str(type_enum e) {
  switch(e) {
      case STRING:
          return "STRING";
      case BOOL:
          return "BOOL";
      case LONG:
          return "LONG";
      case EMPTY:
          return "EMPTY";
      case STRINGLIST:
          return "STRINGLIST";
  }
  return "UNKNOWN";
}

struct string_list_struct {
  const char** strings;
  int size;
};

typedef struct string_list_struct string_list;

struct pair_struct {
  const char* key;
  type_enum type;
  union {
      const char* str;
      int b;
      long l;
      string_list str_l;
  };
};

typedef struct pair_struct pair;

struct list_struct {
  pair* pairs;
  int length;
};

typedef struct list_struct list;
