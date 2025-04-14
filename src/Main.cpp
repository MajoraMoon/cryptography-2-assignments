
#include "task.h"

/**
 *
 * I am using the Botan library for encryption.
 * Using the official binariese from the arch repo and the window version was
 * compiled with mingW and put on the C-Drive, so in "C:/Botan" directory. Botan
 * does not provide any official precompiled Binaries, so it can take some time
 * to set it up and use this code. That is also the reason why the
 * CmakeLists.txt has hardcoded directories on windows. I also did not want to
 * set up global Paths for every single binary file. As long it compiles for the
 * tasks it's fine :D
 */

int main(int argc, char const *argv[]) {
  // Aufgabe 1 - a
  execute_assignment_1_a();

  // Aufgabe 1 - b
  execute_assignment_1_b();

  // Aufgabe 2 - a
  execute_assignment_2_a();

  // Aufgabe 2 - b
  execute_assignemt_2_b();

  // Aufgabe 2 - c
  execute_assignemt_2_c();

  return 0;
}
