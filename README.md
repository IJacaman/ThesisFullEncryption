# Full Colour Image Encryption (Baseline)

C++ implementation of the full-image encryption variant used as the baseline in my master's thesis and journal article. It applies the same operations as the selective scheme in [ThesisSelectiveEncryption](https://github.com/IJacaman/ThesisSelectiveEncryption) to every block of the image, so that the security and speed of selective and full encryption can be compared. The MATLAB evaluation scripts are in [MatLab](https://github.com/IJacaman/MatLab).

**Paper:** https://doi.org/10.54963/jic.v4i2.1616

## Method

Every block of the colour image is encrypted: the most significant bit of each byte is XORed, the 7th–5th bits are swapped between the red, green and blue channels, pixels are scrambled column-wise with the Fisher–Yates shuffle, and each byte is XORed with the previous one plus a random byte in a CBC-like mode. Decryption applies the inverse steps with the same key.

## Requirements

- Windows with Visual Studio 2019 or later and the "Desktop development with C++" workload
- Crypto++ (included in `cryptopp/`)

## Build

1. Open `ImageProcessing.sln` in Visual Studio.
2. Select `Release | x64` and build. Build Crypto++ first if Visual Studio reports a missing `cryptlib` library.

## Run

[FILL IN: how to start the program, e.g. `ImageProcessing.exe <input image> <key>`, or which variables in which source file set the input image and key.]

Input images are read from `images/`. [FILL IN: where encrypted and decrypted images are written.]

## Reproducing the results

Encrypt the same test images with the same key as the selective scheme, run the MATLAB scripts on both sets of outputs, and compare the security metrics and execution times, as in the paper's comparison tables.

## Repository layout

| Path | Contents |
|---|---|
| `ImageProcessing/` | Source code of the full-image scheme |
| `cryptopp/` | Bundled Crypto++ library |
| `images/` | Test images |
| `ImageProcessing.sln` | Visual Studio solution |

## Known limitations

The random values used during encryption come from an external pseudo-random source (thesis, Section 5.2). A cryptographically secure generator should be used in any real deployment.

## Status

This repository contains the thesis code as recovered from the last working backup after the original development machine was lost. It has not yet been rebuilt on a clean machine. If you run into a build problem, please open an issue.

## Citation

If you use this code, please cite:

> Jacaman, I., & Farajallah, M. (2025). Selective Color Image Encryption Based on MSB and Sensitive Bits. *Journal of Intelligent Communication*, 4(2), 115–140. https://doi.org/10.54963/jic.v4i2.1616

```bibtex
@article{jacaman2025selective,
  author  = {Jacaman, Issa and Farajallah, Mousa},
  title   = {Selective Color Image Encryption Based on {MSB} and Sensitive Bits},
  journal = {Journal of Intelligent Communication},
  volume  = {4},
  number  = {2},
  pages   = {115--140},
  year    = {2025},
  doi     = {10.54963/jic.v4i2.1616}
}
```

Master's thesis (Palestine Polytechnic University, 2023), supervised by Dr. Mousa Farajallah: https://scholar.ppu.edu/handle/123456789/9125

## License

MIT License (see `LICENSE`). Crypto++ keeps its own license.
