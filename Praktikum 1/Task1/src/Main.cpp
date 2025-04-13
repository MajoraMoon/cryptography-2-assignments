
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
  execute_task_1_a();

  // Aufgabe 1 - b
  execute_task_1_b();
  return 0;
}
