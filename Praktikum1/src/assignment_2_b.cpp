#include <task.h>

/**
 *
 * C.2 DSA-Signaturen:
 *  Implementieren Sie den DSA-Signaturalgorithmus mit der Hash-
 * funktion SHA-256. Signieren Sie zufällig gewählte Nachrichten
 * mit dem DSA-Signaturalgorithmus und prüfen Sie, dass der Ver-
 * ifikationsalgorithmus die Signaturen korrekt verifiziert.
 *
 */

/**
 *
 * Schlüsselgenerator nach FIPS 186-4 (also in der Vorlesung ist der Generator
 * alpha so wie ich es verstanden habe)
 * genrate_alphaa mit zwei a, weil sonst der Linker meckert und es ist unnötig
 * dafür extra eine datei bzw Klasse zu kreieren
 *
 */
Botan::BigInt generate_alphaa(Botan::BigInt &p, Botan::BigInt &q,
                              Botan::RandomNumberGenerator &rng) {

  Botan::BigInt e = (p - 1) / q;

  Botan::BigInt h, g;

  do {
    // Botan inkludiert die unter UND obergrenze, deshalb muss hier 2 anstatt 1
    // stehen, weil h nicht gleich größer/kleiner 1 ist , sondern echt
    // größer/kleiner 1
    h = Botan::BigInt::random_integer(rng, 2, p - 2);

    g = Botan::power_mod(h, e, p);

  } while (g == 1);

  return g;
}

void execute_assignemt_2_b() {
  Botan::AutoSeeded_RNG rng;

  // Primzahlen generieren nach FIPS 186-4
  const size_t p_bits = 3072;
  const size_t q_bits = 256;

  // generiere q
  Botan::BigInt q = Botan::random_prime(rng, q_bits);

  // generiere p
  Botan::BigInt p;
  // Dieser For loop ist äquivalent zu "while(true)", also ein infinity for loop
  for (;;) {
    // random zahl c wird genommen
    Botan::BigInt c = Botan::BigInt::random_integer(
        rng, 1, Botan::BigInt::power_of_2(p_bits - q_bits));
    // p - 1 = c * q also ist p = c * q + 1
    p = c * q + 1;

    if (p.bits() == p_bits && Botan::is_prime(p, rng)) {
      break;
    }
  }

  Botan::BigInt alpha = generate_alphaa(p, q, rng);

  Botan::BigInt a = Botan::BigInt::random_integer(rng, 1, q - 1);
  Botan::BigInt beta = Botan::power_mod(alpha, a, p);

  // Ab hier wird es dann wirklich signiert, bzw Aufgabe 2b ab hier.

  // Beispielnachricht
  std::string message = "Testnachricht zur DSA Signierung.";

  // Hash der Nachricht mit SHA-256
  // die "create" function der HashFunction klasse gib ein unique_ptr zurück,
  // also speicher ich es auch in einem ab.
  std::unique_ptr<Botan::HashFunction> hash_fn(
      Botan::HashFunction::create("SHA-256"));

  // Die "update" funktion ist von der Oberklasse "Buffered_Computation".
  // Die Funktion nimmt ein uint8_t (also unsigned char) auf. "reinterpret_cast"
  // castet einfach die Nachricht auf uint8_t ohne zu checken ob es überhaupt
  // geht. In dem falle geht es ohne Probleme, da ein String am Ende nur ein
  // array von chars ist
  hash_fn->update(reinterpret_cast<const uint8_t *>(message.data()),
                  message.size());

  // die "final" funktion sagt :
  // " Complete the computation and retrieve the final result."
  // Nutzen der secure_vector implementation
  Botan::secure_vector<uint8_t> secure_digest = hash_fn->final();

  // Erstellt einen std::vector<uint8_t> durch Kopieren der Elemente
  std::vector<uint8_t> digest(secure_digest.begin(), secure_digest.end());

  // Wandelt den Hash in einen BigInt um und reduziere mod q .DSA fordert eine
  // Reduktion, falls Hash größer als q ist
  Botan::BigInt H = Botan::BigInt::decode(digest) % q;

  // Wähle zufällig k ∈ [1, q-1] (Also das hier ist die geheime Zufallszahl r
  // zwischen gleich 1 und q - 1)
  Botan::BigInt k = Botan::BigInt::random_integer(rng, 1, q - 1);

  // γ = (α^k mod p) mod q
  Botan::BigInt gamma = Botan::power_mod(alpha, k, p) % q;

  // Berechnung von der modularen Inversen von k modulo q: k⁻¹ (in den Folien
  // r⁻¹ --> für k)
  Botan::BigInt k_inv = Botan::inverse_mod(k, q);

  //  δ = (H + a·γ) * k⁻¹ mod q --> Hier wende ich nur die Formel an tbh
  Botan::BigInt delta = ((H + a * gamma) * k_inv) % q;

  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\nAufgabe 2 b)" << "\n";
  std::cout << "DSA-Signatur:\n";
  std::cout << "γ (gamma): " << gamma.to_hex_string() << "\n";
  std::cout << "δ (delta): " << delta.to_hex_string() << "\n\n";

  // Berechnung von  δ⁻¹ modulo q
  Botan::BigInt delta_inv = Botan::inverse_mod(delta, q);

  // e1 = (H * δ⁻¹) mod q
  Botan::BigInt e1 = (H * delta_inv) % q;

  // e2 = (γ * δ⁻¹) mod q
  Botan::BigInt e2 = (gamma * delta_inv) % q;

  // Berechne v = ( (α^e1 * β^e2) mod p ) mod q
  Botan::BigInt v =
      (Botan::power_mod(alpha, e1, p) * Botan::power_mod(beta, e2, p)) % p;
  v = v % q;

  std::cout << "Verifikation:\n";
  std::cout << "v: " << v.to_hex_string() << "\n";
  std::cout << "γ: " << gamma.to_hex_string() << "\n";

  // Also wenn V_k(gamma, delta) = gamma ist, dann ist die Sigantur richtig
  if (v == gamma)
    std::cout << "Signatur verifiziert erfolgreich!\n";
  else
    std::cout << "Signatur-Verifikation fehlgeschlagen!\n";

  // Manipulierte Signatur
  Botan::BigInt gamma_modified = gamma + 1; // Gamma wird mit 1 addiert

  // Berechnung der verifikation für die manipulierte Signatur (Mit den gleichen
  // Parameter)
  delta_inv = Botan::inverse_mod(delta, q); // gleiche Variable wiederverwenden
  e1 = (H * delta_inv) % q;

  e2 = (gamma_modified * delta_inv) % q;

  Botan::BigInt v_modified =
      (Botan::power_mod(alpha, e1, p) * Botan::power_mod(beta, e2, p)) % p;
  v_modified %= q;

  std::cout << "\nManipulierte Signatur:\n";
  std::cout << "v (modifiziert): " << v_modified.to_hex_string() << "\n";
  std::cout << "γ (original): " << gamma.to_hex_string() << "\n";
  if (v_modified == gamma) {
    std::cout << "Fehler: Manipulierte Signatur wurde verifiziert!\n";
  } else {
    std::cout << "Manipulierte Signatur wird nicht verifiziert \n";
  }
}
