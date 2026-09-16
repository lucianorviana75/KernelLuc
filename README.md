# KernioLuc OS

🧠 **KernioLuc OS** é um sistema operacional educacional desenvolvido em C e Assembly (x86 32-bit), criado para explorar os fundamentos de desenvolvimento de kernels *bare-metal*, drivers de hardware e renderização gráfica.

---

## 🚀 Funcionalidades Atuais

* **Bootloader e Modo Protegido:** Transição do Bootloader Real Mode (16-bit) para Protected Mode (32-bit).
* **Interface Gráfica (VGA Modo 13h):** Renderização de texto pixel a pixel com suporte à tabela ASCII completa de 128 caracteres.
* **Driver de Teclado PS/2:** Mapeamento de scancodes para captura de texto e teclas de controle em tempo real.
* **Shell Interativo:** Terminal customizado com suporte a comandos e feedback visual na tela.

---

## 🔬 Tópicos de Estudo & Arquitetura

- [x] Bootloader e inicialização de sistema em Assembly x86
- [x] Alternância para Modo Protegido (32-bit)
- [x] Driver Gráfico VGA $320 \times 200$ (256 cores)
- [x] Driver de Teclado PS/2 (E/S via portas `0x60` e `0x64`)
- [ ] Gerenciamento de Memória Dinâmica (Paging / Heap Allocator)
- [ ] Tabela de Interrupções (IDT) e ISRs
- [ ] Multitasking e Troca de Contexto
- [ ] Sistema de Arquivos (VFS / RAMDisk)

---

## 🛠️ Ferramentas Utilizadas

* **Linguagens:** C e Assembly NASM (x86 32-bit)
* **Compilador e Ferramentas:** `gcc`, `nasm`, `ld`, `make`
* **Emulador:** QEMU (`qemu-system-i386`)

---

## 💻 Como Compilar e Executar

### Pré-requisitos
No Linux (Ubuntu/Debian), instale as ferramentas necessárias:
```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86
