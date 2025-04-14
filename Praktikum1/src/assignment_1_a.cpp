#include "task.h"

/**
 *
 * C.1 Universelle Faelschung von RSA-Signaturen:
 *
 * (a) Erzeugen Sie einen eigenen 3000-Bit RSA-Schl¨ussel (wie auch
 * schon bei der Angewandten Kryptographie 1, Praktikum 5). Geben
 * Sie alle verwendeten RSA-Schl¨usselparameter aus.
 *
 * (Ich mag es auf english zu programmieren und auch kommentare zu schreiben was
 * das angeht, auch wenn die Aufgaben auf deutsch sind)
 */

void execute_assignment_1_a() {

  // random number generator
  Botan::AutoSeeded_RNG rng;

  // dynamic Array with bytes "uint8_t --> unsigned char" and then it gets
  // filled with the "rng.randomize" function
  std::vector<uint8_t> random_bytes(16);
  rng.randomize(random_bytes.data(), random_bytes.size());

  // just gives out the random bytes on the console to show :)
  std::cout << "Zufaellige Bytes (hex): ";
  for (auto byte : random_bytes) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
  }
  std::cout << std::dec << std::endl;

  // generates RSA Private Key object
  Botan::RSA_PrivateKey private_key(rng, 3000);

  std::cout << "Aufgabe 1 a)"
            << "\n";
  // getting the private key information through getter
  std::cout << "Modulus (n): " << private_key.get_n() << "\n\n";
  std::cout << "Oeffentlicher Exponent (e): " << private_key.get_e() << "\n\n";
  std::cout << "Privater Exponent (d): " << private_key.get_d() << "\n\n";
  std::cout << "Primzahl p: " << private_key.get_p() << "\n\n";
  std::cout << "Primzahl q: " << private_key.get_q() << "\n\n";
  std::cout << "d mod (p-1): " << private_key.get_d1() << "\n\n";
  std::cout << "d mod (q-1): " << private_key.get_d2() << "\n\n";
  std::cout << "q^-1 mod p: " << private_key.get_c() << "\n";
}