int main(int argc, char **argv) {
  long long x = argc;
  (void)argv;
  return x * x * x % 1234567890;
}
