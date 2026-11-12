# Full Colour Image Encryption (Baseline)

C++ implementation of the full-image encryption variant used as the baseline in my master's thesis and journal article. It encrypts every pixel of a colour image, so its security and speed can be compared with the selective method in [ThesisSelectiveEncryption](https://github.com/IJacaman/ThesisSelectiveEncryption).

**Paper:** https://doi.org/10.54963/jic.v4i2.1616

## Requirements

- Windows with Visual Studio 2019 or later, "Desktop development with C++" workload <!-- TODO: confirm -->
- Crypto++ <!-- TODO: bundled or installed separately? -->

## Build

1. Open the `.sln` file in Visual Studio. <!-- TODO: name the solution file -->
2. Select `Release | x64` and build.

## Run

<!-- TODO: exact command or where input paths and key are set -->

## Reproducing the paper's results

Run the program on the same test images and key as the selective method, then compare security metrics and execution time against the selective variant, as in the paper's comparison tables. <!-- TODO: name the tables -->

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

MIT License (see `LICENSE`). Bundled third-party libraries keep their own licenses.
