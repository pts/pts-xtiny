__attribute__((constructor)) void myctorgcs(void) {}

extern void magic(void);
void callmagic(void) { magic(); }
int uanswer = 42;
/*int main(void) { return 0; }*/

