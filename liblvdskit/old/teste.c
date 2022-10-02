#include <cs50.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  string oi, cv = "Como vai";

  oi = get_string("Como Vai:");

  oi[4] = '1';

  printf("%s", oi);

  return 0;
}
