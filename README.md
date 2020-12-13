# Trab2-Redes
##Membros do projeto
Gabriela Barion Vidal - 4277535
Pedro Ramos Cunha - 10892248
Rodrigo Bragato Piva - 10684573

##Descrição do projeto
O projeto consiste em simular o funcionamento da camada de enlace de uma transmissão.

Ele foi desenvolvido para a matéria de SSC0641 - Redes de Computadores ministrada pelo professor Rodolfo Ipolito Meneguette para a turma de Engenharia de Computação da USP de São Carlos.

##Execução do código
Para rodar o código, basta baixar o arquivo main.cpp e possuir um compilador de C++ (linguagem em que foi escrito o arquivo).

Dados do Compilador

    ramos@Pedro:~$ gcc --version
    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
    Copyright (C) 2019 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Dados do Sistema Operacional

    ramos@Pedro:~$ lsb_release -a
    No LSB modules are available.
    Distributor ID:	Ubuntu
    Description:	Ubuntu 20.04.1 LTS
    Release:	20.04
    Codename:   focal

Dados da máquina

    ramos@Pedro:~$ lscpu
    Architecture:                    x86_64
    CPU op-mode(s):                  32-bit, 64-bit
    Byte Order:                      Little Endian
    Address sizes:                   48 bits physical, 48 bits virtual
    CPU(s):                          4
    On-line CPU(s) list:             0-3
    Thread(s) per core:              2
    Core(s) per socket:              2
    Socket(s):                       1
    NUMA node(s):                    1
    Vendor ID:                       AuthenticAMD
    CPU family:                      21
    Model:                           101
    Model name:                      AMD A10-9600P RADEON R5, 10 COMPUTE CORES 4C+6G
    Stepping:                        1
    Frequency boost:                 enabled
    CPU MHz:                         1284.038
    CPU max MHz:                     2400.0000
    CPU min MHz:                     1200.0000
    BogoMIPS:                        4791.03
    Virtualisation:                  AMD-V
    L1d cache:                       64 KiB
    L1i cache:                       192 KiB
    L2 cache:                        2 MiB
    NUMA node0 CPU(s):               0-3


    ramos@Pedro:~$sudo lshw -class CPU    
    *-cpu                     
       description: CPU
       product: AMD A10-9600P RADEON R5, 10 COMPUTE CORES 4C+6G
       vendor: Advanced Micro Devices [AMD]
       physical id: 4
       bus info: cpu@0
       version: AMD A10-9600P RADEON R5, 10 COMPUTE CORES 4C+6G
       serial: Unknown
       slot: Socket FP4
       size: 1325MHz
       capacity: 2400MHz
       width: 64 bits
       clock: 100MHz
       capabilities: lm fpu fpu_exception wp vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp x86-64 constant_tsc rep_good acc_power nopl nonstop_tsc cpuid extd_apicid aperfmperf pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs xop skinit wdt lwp fma4 tce nodeid_msr tbm topoext perfctr_core perfctr_nb bpext ptsc mwaitx cpb hw_pstate ssbd vmmcall fsgsbase bmi1 avx2 smep bmi2 xsaveopt arat npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold avic v_vmsave_vmload vgif overflow_recov cpufreq
       configuration: cores=4 enabledcores=4 threads=4