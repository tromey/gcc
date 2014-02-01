/* { dg-do compile } */
/* { dg-options "-ftrack-macro-expansion=0" } */

extern void abort (void);

#define A(N) __attribute__((address_space (N)))
#define force __attribute__((force))

struct S
{
  int x;
};

A(0) int *a;
A(1) int *b;

/* This is a valid "NULL".  */
#define Nil0 ((void *) 0)
/* This "NULL" can't be converted to other address spaces.  */
#define Nil1 ((A(1) void *) 0)

int *ok1;

int *bad1;

force int *ok2;
int *ok3;

int *bad_nil = ((A(1) int *) 0); /* { dg-warning "address space" } */

A(1) struct S *sptr;

A(1) A(2) void *err1;		/* { dg-error "conflicting value" } */
typedef A(1) void *ptr1type;
A(2) ptr1type err2;		/* { dg-error "conflicting value" } */

A("hi bob") void *err3;		/* { dg-error "integer constant" } */
A(3) int err4;			/* { dg-error "pointer types" } */

int func_aspace_0 (A(0) int *a)
{
  return *a;
}

int func_aspace_0f (force int *a)
{
  return *a;
}

int func_aspace_1 (A(1) int *a)
{
  return *a;
}

A(0) int *func_ok1 (A(0) int *p) { return p; }
A(1) int *func_bad1 (A(0) int *p) { return p; }	/* { dg-warning "address space" } */
force A(1) int *func_ok2 (A(0) int *p) { return p; } /* { dg-warning "address space" } */

A(0) int *func_nil_ok1 (void) { return Nil0; }
A(1) int *func_nil_ok2 (void) { return Nil0; }
force A(1) int *func_nil_ok3 (void) { return Nil0; }
A(0) int *func_nil_bad (void) { return Nil1; } /* { dg-warning "address space" } */
A(1) int *func_nil_ok5 (void) { return Nil1; }
force A(1) int *func_nil_ok6 (void) { return Nil1; }

int
use (int x)
{
  if (!x)
    abort ();
  return x;
}

int main(void)
{
  func_aspace_0 (a);
  func_aspace_0f (a);
  func_aspace_1 (a);		/* { dg-warning "address space" } */
  func_aspace_0 (b);		/* { dg-warning "address space" } */
  func_aspace_0f (b);
  func_aspace_1 (b);

  ok1 = a;
  ok1 = a + 0;
  bad1 = b;			/* { dg-warning "address space" } */
  bad1 = b + 0;			/* { dg-warning "address space" } */
  ok2 = b;			/* { dg-warning "address space" } */
  ok2 = b + 0;			/* { dg-warning "address space" } */
  ok3 = (force int *) b;

  func_aspace_0 (Nil0);
  func_aspace_0f (Nil0);
  func_aspace_1 (Nil0);
  func_aspace_0 (Nil1);		/* { dg-warning "address space" } */
  func_aspace_0f (Nil1);
  func_aspace_1 (Nil1);

  use (b == a);			/* { dg-warning "address space" } */
  use (ok1 == a);
  use (ok2 == a);
  use (ok1 == b);		/* { dg-warning "address space" } */
  use (ok2 == b);		/* { dg-warning "address space" } */
  use (b == a);			/* { dg-warning "address space" } */
  use (ok1 != a);
  use (ok2 != a);
  use (ok1 != b);		/* { dg-warning "address space" } */
  use (ok2 != b);		/* { dg-warning "address space" } */

  use (a == Nil0);
  use (b == Nil0);
  use (a == Nil1);		/* { dg-warning "address space" } */
  use (b == Nil1);
  use (Nil0 == a);
  use (Nil0 == b);
  use (Nil1 == a);		/* { dg-warning "address space" } */
  use (Nil1 == b);

  func_aspace_0f (use (23) ? a : b); /* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? a : Nil0);
  func_aspace_0f (use (23) ? a : Nil1);	/* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? Nil0 : a);
  func_aspace_0f (use (23) ? Nil1 : a);	/* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? b : a); /* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? b : Nil0);	/* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? b : Nil1);
  func_aspace_0f (use (23) ? Nil0 : b);	/* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? Nil1 : b);
  func_aspace_0f (use (23) ? b : ok2); /* { dg-warning "address space" } */
  func_aspace_0f (use (23) ? ok2 : b); /* { dg-warning "address space" } */

  a = &sptr->x;			/* { dg-warning "address space" } */
  b = &sptr->x;
}
