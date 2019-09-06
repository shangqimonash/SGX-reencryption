# SGX Development

## Environment Setup

### SGX Package

Intel SGX package for Linux includes three components:

- Intel SGX driver
- Intel SGX platform software (PSW)
- Intel SGX SDK

Refer to Intel's [installation guide document][Installation] to get more details.

After installing the driver, you should check the status of your SGX driver via the following command:

```bash
ls /dev | grep isgx
```

If the SGX driver is successfully installed and loaded, it will return `isgx`. However, if the driver is not loaded, the above command returns nothing. One of the most common causes of this error is the SecureBoot mechanism in BIOS. This mechanism blocks the operating system to load any unsigned driver. To solve this problem, you can either:

- disable the SecureBoot in BIOS
- [sign the SGX driver][Sign SGX Module] by yourselves

### Eclipse Plugin

Intel only provides an Eclipse plugin under the Linux platform: the [installation guide document][Installation] also includes the detail about how to install the plugin for Eclipse.

[Installation]: https://download.01.org/intel-sgx/linux-2.6/docs/Intel_SGX_Installation_Guide_Linux_2.6_Open_Source.pdf
[Sign SGX Module]: https://gist.github.com/DenisKolodin/da73168f2373d8f2ad34d03ad8e313b4

## SGX Project Structure

Intel offers a [standard template][SGX Project Template] for all SGX projects. We try to explain the structure by referring to the Makefile.

### Makefile

SGX SDK Settings: 

- Set the location of SGX SDK
- Define Modes for SGX
  - **SGX_MODE=HW**: hardware mode, must run on the SGX-enabled CPU, the driver/PSW/SDK should be installed correctly
  - **SGX_MODE=SIM**: simulator mode, only SDK is needed
- Define Compilation Target
  - **SGX_DEBUG**: No compiler optimisation, debug symbol included, run on enclave-debug mode (suitable for development)
  - **SGX_PRERELEASE**: compiler optimisation enabled, no debug symbol, run on enclave-debug mode (suitable for performance test)
  - **SGX_RELEASE**: compiler optimisation enabled, no debug symbol, run on enclave-production mode (we do not use this)

APP Settings:

- Set untrusted cpp files include paths and link flags

Enclave Settings:

- Set trusted cpp files include paths and link flags
- Set target enclave name 
- Specific Enclave Configuration File (**Enclave/Enclave.config.xml**)
  - **ProdID**, **ISVSVN**: use the default value, as we do not have remote attestation
  - **Stack/HeapMaxSize**: restricted by the size of EPC and BIOS, should be 4KB aligned
  - **TCSPolicy**, **TCSNum**: for multi-threading, we set them to 1, as we do not have multithreading in our example
  - **DisableDebug**: set to 0 if debug is needed
  - **MiscSelect** and **MiscMask**: used for future extensions, use the default value, don't change it
- Specific Enclave Linker Script (**Enclave/Enclave.debug.lds (for debug) or Enclave/Enclave.lds (for production)**)

Build Target:

- For **HW_RELEASE**: ${App_Name} \${Enclave_Name}, need to sign enclave with a production key
- For others: ${App_Name} \${Signed_Enclave_Name}, sign with your own private key

You can generate a private key via the following command:

```bash
openssl genrsa -out Enclave_private.pem -3 3072
```

EDL (Enclave Description Language) file:

- Define interface between the untrusted application and the enclave
- Makefile uses Edger8r to generate four files from it

[SGX Project Template]: https://github.com/intel/linux-sgx/tree/master/SampleCode/Cxx11SGXDemo

## SGX Re-encryption Example

The re-encryption example is inspired by the traditional proxy re-encryption scheme. While the original scheme uses public-key primitives, the SGX-based solution only needs symmetric-key encryption schemes.

In the main function, after the enclave setup phase, it initialises two parties: Alice and Bob. Each party register their secret key by calling to an ecall defined in .edl. Then, it sets Bob as the communication peer of Alice.

When Alice wants to send some message to Bob, she encrypts the message locally and sends the message to the enclave for re-encryption (ecall in User::send_to_peer() function).

The enclave retrieves the registered key to decrypt and re-encrypt it without leaking the message to the untrusted server and uses ocall to send the message to the peer.
