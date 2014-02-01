/* { dg-do compile } */
/* { dg-options "-std=c99" } */

extern void abort (void);

#define NO __attribute__((noderef))

struct S {
  int x;
  int y;
};

NO int *a;
NO struct S *s_ptr;

int sizeof_ok = sizeof (*a);
int alignof_ok = __extension__ __alignof__ (*a);
__extension__ __typeof__ (*a) typeof_ok;

typedef NO int *nointptr;
NO nointptr *pp;

NO int err1;			/* { dg-error "pointer types" } */

int g (void)
{
  int g_bad = *a;		    /* { dg-warning "dereference" } */
  int g_sizeof_ok = sizeof (*a);
  int g_alignof_ok = __extension__ __alignof__ (*a);
  __extension__ __typeof__ (*a) g_typeof_ok;
  struct S g_bad_init = { 23, *a }; /* { dg-warning "dereference" } */
  NO int *g_addr_ok = &s_ptr->y;
  NO int *g_addr_ok2 = &*a;
  NO int *g_addr_ok3 = &((*s_ptr).y);
  int g_bad_deref = s_ptr->y;	    /* { dg-warning "dereference" } */
  int *g_bad_conv = a;		    /* { dg-warning "noderef" } */
  int *g_bad_cast = (int *) a;	    /* { dg-warning "noderef" } */
  int *g_cast_ok = (__attribute__ ((force)) int *) a;
  int x;

  x = (*a			    /* { dg-warning "dereference" } */
       + s_ptr->y);		    /* { dg-warning "dereference" } */

  NO int *g_bad_conv2 = &x;	    /* { dg-warning "noderef" } */
  NO int *g_addr_bad = &*&*&*&*&*&*&*&**pp; /* { dg-warning "dereference" } */

  if (s_ptr ? &s_ptr->x : &x)	  /* { dg-warning "conditional expression" } */
    abort ();

  NO int *ptr = (void *) 0;

  return *a;			    /* { dg-warning "dereference" } */
}
