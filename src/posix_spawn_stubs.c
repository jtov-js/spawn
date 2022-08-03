#include "common.h"
#ifdef USE_POSIX_SPAWN

#include <assert.h>
#include <spawn.h>

#define File_actions_val(Val)       Abstract_ptr_val(posix_spawn_file_actions_t, Val)
#define Spawnattr_val(Val)          Abstract_ptr_val(posix_spawnattr_t, Val)
#define Abstract_ptr_val(Typ, Val)  *(Typ *) (Data_abstract_val(Val))

static void custom_finalize_file_actions(value);
static void custom_finalize_spawnattr(value);

static struct custom_operations file_actions_table = {
  "com.janestreet.spawn.posix_spawn_file_actions",
  custom_finalize_file_actions,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default,
  custom_fixed_length_default
};

static struct custom_operations spawnattr_table = {
  "com.janestreet.spawn.posix_spawnattr",
  custom_finalize_spawnattr,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default,
  custom_fixed_length_default
};

CAMLprim value
spawn_file_actions_create(void)
{
  CAMLparam0();
  CAMLlocal1(result);

  posix_spawn_file_actions_t *actions;
  result = caml_alloc_custom_mem(&file_actions_table, sizeof actions, sizeof *actions);
  actions = File_actions_val(result) = malloc(sizeof *actions);

  if (actions == NULL) caml_raise_out_of_memory();

  int err = posix_spawn_file_actions_init(actions);
  if (err != 0) {
    File_actions_val(result) = NULL;
    free(actions);
    unix_error(err, "posix_spawn_file_actions_init", Val_none);
  }

  CAMLreturn(result);
}


CAMLprim value
spawn_spawnattr_create(void)
{
  CAMLparam0();
  CAMLlocal1(result);

  posix_spawnattr_t *attr;
  result = caml_alloc_custom_mem(&spawnattr_table, sizeof attr, sizeof *attr);
  attr = Spawnattr_val(result) = malloc(sizeof *attr);

  if (attr == NULL) caml_raise_out_of_memory();

  int err = posix_spawnattr_init(attr);
  if (err != 0) {
    Spawnattr_val(result) = NULL;
    free(attr);
    unix_error(err, "posix_spawnattr_init", Val_none);
  }

  CAMLreturn(result);
}

static void
custom_finalize_file_actions(value abstract)
{
  posix_spawn_file_actions_t *actions = File_actions_val(abstract);
  if (actions != NULL) {
    posix_spawn_file_actions_destroy(actions);
    free(actions);
  }
}

static void
custom_finalize_spawnattr(value abstract)
{
  posix_spawnattr_t *attr = Spawnattr_val(abstract);
  if (attr != NULL) {
    posix_spawnattr_destroy(attr);
    free(attr);
  }
}

#else // USE_POSIX_SPAWN

/* From https://v2.ocaml.org/manual/intfc.html */
/* static value */
/* val_of_actions_ptr(file_actions_t p) */
/* { */
/*   value result = 1 | (value) p; */
/*   assert(result != (value) p); */
/*   return result; */
/* } */

/* static file_actions_t */
/* actions_ptr_of_val(value v) */
/* { */
/*   return (file_actions_t) (v & ~1); */
/* } */


#endif // USE_POSIX_SPAWN
