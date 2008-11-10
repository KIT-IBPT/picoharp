global DATABASE
DATABASE = struct();
% raw picoharp buffer (counts)
DATABASE.SR_DI_PICO_01_FILL = zeros(1, 65536);

DATABASE.SR_DI_PICO_01_BUCKETS = zeros(1, 936);
DATABASE.SR_DI_PICO_01_BUCKETS_60 = zeros(1, 936);
DATABASE.SR_DI_PICO_01_BUCKETS_180 = zeros(1, 936);
DATABASE.SR_DI_PICO_01_PEAK = 0;
DATABASE.SR_DI_PICO_01_PK_AUTO = 1;
DATABASE.SR_DI_PICO_01_FLUX = 0;
% acquisition time (ms)
DATABASE.SR_DI_PICO_01_TIME = 5000;
DATABASE.SR_DI_PICO_01_MAX_BIN = 0;
DATABASE.SR_DI_PICO_01_SHIFT = 650;
DATABASE.SR_DI_PICO_01_COUNTS_FILL = 0;
DATABASE.SR_DI_PICO_01_COUNTS_5 = 1;
DATABASE.SR_DI_PICO_01_COUNTS_60 = 1;
DATABASE.SR_DI_PICO_01_COUNTS_180 = 1;
DATABASE.LI_RF_MOSC_01_FREQ = 4.99653e8;
DATABASE.SR21C_DI_DCCT_01_CHARGE = 10;
server_1;
