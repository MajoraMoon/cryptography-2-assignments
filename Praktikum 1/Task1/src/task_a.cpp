#include "task.h"

void execute_task_1_a() {

  Botan::AutoSeeded_RNG rng;

  Botan::RSA_PrivateKey private_key(rng, 3000);

  std::cout << "Modulus (n): " << private_key.get_n() << "\n\n";
  std::cout << "Oeffentlicher Exponent (e): " << private_key.get_e() << "\n\n";
  std::cout << "Privater Exponent (d): " << private_key.get_d() << "\n\n";
  std::cout << "Primzahl p: " << private_key.get_p() << "\n\n";
  std::cout << "Primzahl q: " << private_key.get_q() << "\n\n";
  std::cout << "d mod (p-1): " << private_key.get_d1() << "\n\n";
  std::cout << "d mod (q-1): " << private_key.get_d2() << "\n\n";
  std::cout << "q^-1 mod p: " << private_key.get_c() << "\n";
}