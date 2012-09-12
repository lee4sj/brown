/*      */ package javazoom.jl.decoder;
/*      */ 
/*      */ final class LayerIIIDecoder
/*      */   implements FrameDecoder
/*      */ {
/*   44 */   final double d43 = 1.333333333333333D;
/*      */   public int[] scalefac_buffer;
/*   50 */   private int CheckSumHuff = 0;
/*      */   private int[] is_1d;
/*      */   private float[][][] ro;
/*      */   private float[][][] lr;
/*      */   private float[] out_1d;
/*      */   private float[][] prevblck;
/*      */   private float[][] k;
/*      */   private int[] nonzero;
/*      */   private Bitstream stream;
/*      */   private Header header;
/*      */   private SynthesisFilter filter1;
/*      */   private SynthesisFilter filter2;
/*      */   private Obuffer buffer;
/*      */   private int which_channels;
/*      */   private BitReserve br;
/*      */   private III_side_info_t si;
/*      */   private temporaire2[] III_scalefac_t;
/*      */   private temporaire2[] scalefac;
/*      */   private int max_gr;
/*      */   private int frame_start;
/*      */   private int part2_start;
/*      */   private int channels;
/*      */   private int first_channel;
/*      */   private int last_channel;
/*      */   private int sfreq;
/*  228 */   private float[] samples1 = new float[32];
/*  229 */   private float[] samples2 = new float[32];
/*      */ 
/*  586 */   private final int[] new_slen = new int[4];
/*      */ 
/*  743 */   int[] x = new int[1];
/*  744 */   int[] y = new int[1];
/*  745 */   int[] v = new int[1];
/*  746 */   int[] w = new int[1];
/*      */ 
/* 1115 */   int[] is_pos = new int[576];
/* 1116 */   float[] is_ratio = new float[576];
/*      */ 
/* 1441 */   float[] tsOutCopy = new float[18];
/* 1442 */   float[] rawout = new float[36];
/*      */ 
/* 1816 */   private int counter = 0;
/*      */   private static final int SSLIMIT = 18;
/*      */   private static final int SBLIMIT = 32;
/* 1932 */   private static final int[][] slen = { 
/* 1933 */     { 0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4 }, 
/* 1934 */     { 0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3 } };
/*      */ 
/* 1938 */   public static final int[] pretab = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 2 };
/*      */   private SBI[] sfBandIndex;
/* 1943 */   public static final float[] two_to_negative_half_pow = { 1.0F, 0.7071068F, 0.5F, 0.3535534F, 
/* 1944 */     0.25F, 0.1767767F, 0.125F, 0.08838835F, 
/* 1945 */     0.0625F, 0.04419417F, 0.03125F, 0.02209709F, 
/* 1946 */     0.015625F, 0.01104854F, 0.007813F, 0.005524272F, 
/* 1947 */     0.0039063F, 0.002762136F, 0.00195313F, 0.001381068F, 
/* 1948 */     0.000976563F, 0.000690534F, 0.0004882813F, 0.000345267F, 
/* 1949 */     0.0002441406F, 0.0001726335F, 0.0001220703F, 8.631674E-05F, 
/* 1950 */     6.103516E-05F, 4.315837E-05F, 3.051758E-05F, 2.157919E-05F, 
/* 1951 */     1.525879E-05F, 1.078959E-05F, 7.629395E-06F, 5.394797E-06F, 
/* 1952 */     3.814697E-06F, 2.697398E-06F, 1.907349E-06F, 1.348699E-06F, 
/* 1953 */     9.536743E-07F, 6.743496E-07F, 4.768372E-07F, 3.371748E-07F, 
/* 1954 */     2.384186E-07F, 1.685874E-07F, 1.192093E-07F, 8.42937E-08F, 
/* 1955 */     5.960465E-08F, 4.214685E-08F, 2.980232E-08F, 2.107342E-08F, 
/* 1956 */     1.490116E-08F, 1.053671E-08F, 7.450581E-09F, 5.268356E-09F, 
/* 1957 */     3.72529E-09F, 2.634178E-09F, 1.862645E-09F, 1.317089E-09F, 
/* 1958 */     9.313226E-10F, 6.585445E-10F, 4.656613E-10F, 3.292723E-10F };
/*      */ 
/* 1962 */   public static final float[] t_43 = create_t_43();
/*      */ 
/* 1977 */   public static final float[][] io = { 
/* 1978 */     { 1.0F, 0.8408964F, 0.7071068F, 0.5946035F, 
/* 1979 */     0.5F, 0.4204482F, 0.3535534F, 0.2973018F, 
/* 1980 */     0.25F, 0.2102241F, 0.1767767F, 0.1486509F, 
/* 1981 */     0.125F, 0.1051121F, 0.08838835F, 0.07432544F, 
/* 1982 */     0.0625F, 0.05255603F, 0.04419417F, 0.03716272F, 
/* 1983 */     0.03125F, 0.02627801F, 0.02209709F, 0.01858136F, 
/* 1984 */     0.015625F, 0.01313901F, 0.01104854F, 0.00929068F, 
/* 1985 */     0.007813F, 0.006569503F, 0.005524272F, 0.00464534F }, 
/* 1986 */     { 1.0F, 0.7071068F, 0.5F, 0.3535534F, 
/* 1987 */     0.25F, 0.1767767F, 0.125F, 0.08838835F, 
/* 1988 */     0.0625F, 0.04419417F, 0.03125F, 0.02209709F, 
/* 1989 */     0.015625F, 0.01104854F, 0.007813F, 0.005524272F, 
/* 1990 */     0.0039063F, 0.002762136F, 0.00195313F, 0.001381068F, 
/* 1991 */     0.000976563F, 0.000690534F, 0.0004882813F, 0.000345267F, 
/* 1992 */     0.0002441406F, 0.0001726335F, 0.0001220703F, 8.631674E-05F, 
/* 1993 */     6.103516E-05F, 4.315837E-05F, 3.051758E-05F, 2.157919E-05F } };
/*      */ 
/* 1999 */   public static final float[] TAN12 = { 
/* 2000 */     0.0F, 0.2679492F, 0.5773503F, 1.0F, 
/* 2001 */     1.732051F, 3.732051F, 1.0E+11F, -3.732051F, 
/* 2002 */     -1.732051F, -1.0F, -0.5773503F, -0.2679492F, 
/* 2003 */     0.0F, 0.2679492F, 0.5773503F, 1.0F };
/*      */   private static int[][] reorder_table;
/* 2337 */   private static final float[] cs = { 
/* 2338 */     0.8574929F, 0.881742F, 0.9496287F, 0.9833146F, 
/* 2339 */     0.9955178F, 0.9991606F, 0.9998992F, 0.9999932F };
/*      */ 
/* 2343 */   private static final float[] ca = { 
/* 2344 */     -0.5144957F, -0.471732F, -0.3133774F, -0.181913F, 
/* 2345 */     -0.09457419F, -0.04096558F, -0.01419857F, -0.003699975F };
/*      */ 
/* 2365 */   public static final float[][] win = { 
/* 2366 */     { -0.01614122F, -0.0536032F, -0.1007071F, -0.1628082F, 
/* 2367 */     -0.5F, -0.3838874F, -0.6206114F, -1.165976F, 
/* 2368 */     -3.872075F, -4.225629F, -1.519529F, -0.9741648F, 
/* 2369 */     -0.7374408F, -1.207107F, -0.5163616F, -0.4542605F, 
/* 2370 */     -0.4071566F, -0.3696946F, -0.338763F, -0.3124222F, 
/* 2371 */     -0.2893959F, -0.2688008F, -0.5F, -0.2325142F, 
/* 2372 */     -0.2159672F, -0.2000498F, -0.1844949F, -0.1690585F, 
/* 2373 */     -0.1535036F, -0.1375863F, -0.1210392F, -0.2071068F, 
/* 2374 */     -0.08475258F, -0.06415752F, -0.04113117F, -0.01479071F }, 
/* 2376 */     { -0.01614122F, -0.0536032F, -0.1007071F, -0.1628082F, 
/* 2377 */     -0.5F, -0.3838874F, -0.6206114F, -1.165976F, 
/* 2378 */     -3.872075F, -4.225629F, -1.519529F, -0.9741648F, 
/* 2379 */     -0.7374408F, -1.207107F, -0.5163616F, -0.4542605F, 
/* 2380 */     -0.4071566F, -0.3696946F, -0.3390854F, -0.315118F, 
/* 2381 */     -0.2964223F, -0.2818455F, -0.5411961F, -0.2621323F, 
/* 2382 */     -0.2538792F, -0.2329629F, -0.1985273F, -0.1523354F, 
/* 2383 */     -0.0964964F, -0.0334238F, 0.0F, 0.0F, 
/* 2384 */     0.0F, 0.0F, 0.0F, 0.0F }, 
/* 2386 */     { -0.0483008F, -0.1571566F, -0.2832505F, -0.4295375F, 
/* 2387 */     -1.207107F, -0.8242648F, -1.145175F, -1.769529F, 
/* 2388 */     -4.547022F, -3.489053F, -0.7329629F, -0.1507652F, 
/* 2389 */     0.0F, 0.0F, 0.0F, 0.0F, 
/* 2390 */     0.0F, 0.0F, 0.0F, 0.0F, 
/* 2391 */     0.0F, 0.0F, 0.0F, 0.0F, 
/* 2392 */     0.0F, 0.0F, 0.0F, 0.0F, 
/* 2393 */     0.0F, 0.0F, 0.0F, 0.0F, 
/* 2394 */     0.0F, 0.0F, 0.0F, 0.0F }, 
/* 2396 */     { 0.0F, 0.0F, 0.0F, 0.0F, 
/* 2397 */     0.0F, 0.0F, -0.1507651F, -0.7329629F, 
/* 2398 */     -3.489053F, -4.547022F, -1.769529F, -1.145175F, 
/* 2399 */     -0.8313774F, -1.306563F, -0.5414202F, -0.4652897F, 
/* 2400 */     -0.4106699F, -0.3700468F, -0.338763F, -0.3124222F, 
/* 2401 */     -0.2893959F, -0.2688008F, -0.5F, -0.2325142F, 
/* 2402 */     -0.2159672F, -0.2000498F, -0.1844949F, -0.1690585F, 
/* 2403 */     -0.1535036F, -0.1375863F, -0.1210392F, -0.2071068F, 
/* 2404 */     -0.08475258F, -0.06415752F, -0.04113117F, -0.01479071F } };
/*      */   public Sftable sftable;
/* 2431 */   public static final int[][][] nr_of_sfb_block = { { { 6, 5, 5, 5 }, { 9, 9, 9, 9 }, { 6, 9, 9, 9 } }, 
/* 2432 */     { { 6, 5, 7, 3 }, { 9, 9, 12, 6 }, { 6, 9, 12, 6 } }, 
/* 2433 */     { { 11, 10 }, { 18, 18 }, { 15, 18 } }, 
/* 2434 */     { { 7, 7, 7 }, { 12, 12, 12 }, { 6, 15, 12 } }, 
/* 2435 */     { { 6, 6, 6, 3 }, { 12, 9, 9, 6 }, { 6, 12, 9, 6 } }, 
/* 2436 */     { { 8, 8, 5 }, { 15, 12, 9 }, { 6, 18, 9 } } };
/*      */ 
/*      */   public LayerIIIDecoder(Bitstream stream0, Header header0, SynthesisFilter filtera, SynthesisFilter filterb, Obuffer buffer0, int which_ch0)
/*      */   {
/*   88 */     huffcodetab.inithuff();
/*   89 */     this.is_1d = new int[580];
/*   90 */     this.ro = new float[2][32][18];
/*   91 */     this.lr = new float[2][32][18];
/*   92 */     this.out_1d = new float[576];
/*   93 */     this.prevblck = new float[2][576];
/*   94 */     this.k = new float[2][576];
/*   95 */     this.nonzero = new int[2];
/*      */ 
/*   98 */     this.III_scalefac_t = new temporaire2[2];
/*   99 */     this.III_scalefac_t[0] = new temporaire2();
/*  100 */     this.III_scalefac_t[1] = new temporaire2();
/*  101 */     this.scalefac = this.III_scalefac_t;
/*      */ 
/*  104 */     this.sfBandIndex = new SBI[9];
/*  105 */     int[] l0 = { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576 };
/*  106 */     int[] s0 = { 0, 4, 8, 12, 18, 24, 32, 42, 56, 74, 100, 132, 174, 192 };
/*  107 */     int[] l1 = { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 114, 136, 162, 194, 232, 278, 330, 394, 464, 540, 576 };
/*  108 */     int[] s1 = { 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 136, 180, 192 };
/*  109 */     int[] l2 = { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576 };
/*  110 */     int[] s2 = { 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192 };
/*      */ 
/*  112 */     int[] l3 = { 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162, 196, 238, 288, 342, 418, 576 };
/*  113 */     int[] s3 = { 0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192 };
/*  114 */     int[] l4 = { 0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156, 190, 230, 276, 330, 384, 576 };
/*  115 */     int[] s4 = { 0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192 };
/*  116 */     int[] l5 = { 0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82, 102, 126, 156, 194, 240, 296, 364, 448, 550, 576 };
/*  117 */     int[] s5 = { 0, 4, 8, 12, 16, 22, 30, 42, 58, 78, 104, 138, 180, 192 };
/*      */ 
/*  119 */     int[] l6 = { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576 };
/*  120 */     int[] s6 = { 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192 };
/*  121 */     int[] l7 = { 0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576 };
/*  122 */     int[] s7 = { 0, 4, 8, 12, 18, 26, 36, 48, 62, 80, 104, 134, 174, 192 };
/*  123 */     int[] l8 = { 0, 12, 24, 36, 48, 60, 72, 88, 108, 132, 160, 192, 232, 280, 336, 400, 476, 566, 568, 570, 572, 574, 576 };
/*  124 */     int[] s8 = { 0, 8, 16, 24, 36, 52, 72, 96, 124, 160, 162, 164, 166, 192 };
/*      */ 
/*  126 */     this.sfBandIndex[0] = new SBI(l0, s0);
/*  127 */     this.sfBandIndex[1] = new SBI(l1, s1);
/*  128 */     this.sfBandIndex[2] = new SBI(l2, s2);
/*      */ 
/*  130 */     this.sfBandIndex[3] = new SBI(l3, s3);
/*  131 */     this.sfBandIndex[4] = new SBI(l4, s4);
/*  132 */     this.sfBandIndex[5] = new SBI(l5, s5);
/*      */ 
/*  134 */     this.sfBandIndex[6] = new SBI(l6, s6);
/*  135 */     this.sfBandIndex[7] = new SBI(l7, s7);
/*  136 */     this.sfBandIndex[8] = new SBI(l8, s8);
/*      */ 
/*  139 */     if (reorder_table == null) {
/*  140 */       reorder_table = new int[9];
/*  141 */       for (int i = 0; i < 9; i++) {
/*  142 */         reorder_table[i] = reorder(this.sfBandIndex[i].s);
/*      */       }
/*      */     }
/*      */ 
/*  146 */     int[] ll0 = { 0, 6, 11, 16, 21 };
/*  147 */     int[] ss0 = { 0, 6, 12 };
/*  148 */     this.sftable = new Sftable(ll0, ss0);
/*      */ 
/*  152 */     this.scalefac_buffer = new int[54];
/*      */ 
/*  155 */     this.stream = stream0;
/*  156 */     this.header = header0;
/*  157 */     this.filter1 = filtera;
/*  158 */     this.filter2 = filterb;
/*  159 */     this.buffer = buffer0;
/*  160 */     this.which_channels = which_ch0;
/*      */ 
/*  162 */     this.frame_start = 0;
/*  163 */     this.channels = (this.header.mode() == 3 ? 1 : 2);
/*  164 */     this.max_gr = (this.header.version() == 1 ? 2 : 1);
/*      */ 
/*  166 */     this.sfreq = 
/*  168 */       (this.header.sample_frequency() + (
/*  168 */       this.header.version() == 2 ? 6 : this.header.version() == 1 ? 3 : 
/*  168 */       0));
/*      */ 
/*  170 */     if (this.channels == 2)
/*      */     {
/*  172 */       switch (this.which_channels)
/*      */       {
/*      */       case 1:
/*      */       case 3:
/*  176 */         this.first_channel = (this.last_channel = 0);
/*  177 */         break;
/*      */       case 2:
/*  180 */         this.first_channel = (this.last_channel = 1);
/*  181 */         break;
/*      */       case 0:
/*      */       default:
/*  185 */         this.first_channel = 0;
/*  186 */         this.last_channel = 1; break;
/*      */       }
/*      */ 
/*      */     }
/*      */     else
/*      */     {
/*  192 */       this.first_channel = (this.last_channel = 0);
/*      */     }
/*      */ 
/*  195 */     for (int ch = 0; ch < 2; ch++) {
/*  196 */       for (int j = 0; j < 576; j++)
/*  197 */         this.prevblck[ch][j] = 0.0F;
/*      */     }
/*  199 */     this.nonzero[1] = 576; this.nonzero[0] = 576;
/*      */ 
/*  201 */     this.br = new BitReserve();
/*  202 */     this.si = new III_side_info_t();
/*      */   }
/*      */ 
/*      */   public void seek_notify()
/*      */   {
/*  210 */     this.frame_start = 0;
/*  211 */     for (int ch = 0; ch < 2; ch++)
/*  212 */       for (int j = 0; j < 576; j++)
/*  213 */         this.prevblck[ch][j] = 0.0F;
/*  214 */     this.br = new BitReserve();
/*      */   }
/*      */ 
/*      */   public void decodeFrame()
/*      */   {
/*  219 */     decode();
/*      */   }
/*      */ 
/*      */   public void decode()
/*      */   {
/*  233 */     int nSlots = this.header.slots();
/*      */ 
/*  240 */     get_side_info();
/*      */ 
/*  242 */     for (int i = 0; i < nSlots; i++) {
/*  243 */       this.br.hputbuf(this.stream.get_bits(8));
/*      */     }
/*  245 */     int main_data_end = this.br.hsstell() >>> 3;
/*      */     int flush_main;
/*  247 */     if ((flush_main = this.br.hsstell() & 0x7) != 0) {
/*  248 */       this.br.hgetbits(8 - flush_main);
/*  249 */       main_data_end++;
/*      */     }
/*      */ 
/*  252 */     int bytes_to_discard = this.frame_start - main_data_end - 
/*  253 */       this.si.main_data_begin;
/*      */ 
/*  255 */     this.frame_start += nSlots;
/*      */ 
/*  257 */     if (bytes_to_discard < 0) {
/*  258 */       return;
/*      */     }
/*  260 */     if (main_data_end > 4096) {
/*  261 */       this.frame_start -= 4096;
/*  262 */       this.br.rewindNbytes(4096);
/*      */     }
/*      */ 
/*  265 */     for (; bytes_to_discard > 0; bytes_to_discard--) {
/*  266 */       this.br.hgetbits(8);
/*      */     }
/*  268 */     for (int gr = 0; gr < this.max_gr; gr++)
/*      */     {
/*  270 */       for (int ch = 0; ch < this.channels; ch++) {
/*  271 */         this.part2_start = this.br.hsstell();
/*      */ 
/*  273 */         if (this.header.version() == 1)
/*  274 */           get_scale_factors(ch, gr);
/*      */         else {
/*  276 */           get_LSF_scale_factors(ch, gr);
/*      */         }
/*  278 */         huffman_decode(ch, gr);
/*      */ 
/*  280 */         dequantize_sample(this.ro[ch], ch, gr);
/*      */       }
/*      */ 
/*  283 */       stereo(gr);
/*      */ 
/*  285 */       if ((this.which_channels == 3) && (this.channels > 1)) {
/*  286 */         do_downmix();
/*      */       }
/*  288 */       for (ch = this.first_channel; ch <= this.last_channel; ch++)
/*      */       {
/*  290 */         reorder(this.lr[ch], ch, gr);
/*  291 */         antialias(ch, gr);
/*      */ 
/*  295 */         hybrid(ch, gr);
/*      */ 
/*  300 */         for (int sb18 = 18; sb18 < 576; sb18 += 36) {
/*  301 */           for (int ss = 1; ss < 18; ss += 2)
/*  302 */             this.out_1d[(sb18 + ss)] = (-this.out_1d[(sb18 + ss)]);
/*      */         }
/*  304 */         if ((ch == 0) || (this.which_channels == 2))
/*  305 */           for (int ss = 0; ss < 18; ss++) {
/*  306 */             int sb = 0;
/*  307 */             for (sb18 = 0; sb18 < 576; sb18 += 18) {
/*  308 */               this.samples1[sb] = this.out_1d[(sb18 + ss)];
/*      */ 
/*  310 */               sb++;
/*      */             }
/*  312 */             this.filter1.input_samples(this.samples1);
/*  313 */             this.filter1.calculate_pcm_samples(this.buffer);
/*      */           }
/*      */         else {
/*  316 */           for (int ss = 0; ss < 18; ss++) {
/*  317 */             int sb = 0;
/*  318 */             for (sb18 = 0; sb18 < 576; sb18 += 18) {
/*  319 */               this.samples2[sb] = this.out_1d[(sb18 + ss)];
/*      */ 
/*  321 */               sb++;
/*      */             }
/*  323 */             this.filter2.input_samples(this.samples2);
/*  324 */             this.filter2.calculate_pcm_samples(this.buffer);
/*      */           }
/*      */ 
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  335 */     this.counter += 1;
/*  336 */     this.buffer.write_buffer(1);
/*      */   }
/*      */ 
/*      */   private boolean get_side_info()
/*      */   {
/*  358 */     if (this.header.version() == 1)
/*      */     {
/*  361 */       this.si.main_data_begin = this.stream.get_bits(9);
/*  362 */       if (this.channels == 1)
/*  363 */         this.si.private_bits = this.stream.get_bits(5);
/*  364 */       else this.si.private_bits = this.stream.get_bits(3);
/*      */ 
/*  366 */       for (int ch = 0; ch < this.channels; ch++) {
/*  367 */         this.si.ch[ch].scfsi[0] = this.stream.get_bits(1);
/*  368 */         this.si.ch[ch].scfsi[1] = this.stream.get_bits(1);
/*  369 */         this.si.ch[ch].scfsi[2] = this.stream.get_bits(1);
/*  370 */         this.si.ch[ch].scfsi[3] = this.stream.get_bits(1);
/*      */       }
/*      */ 
/*  373 */       for (int gr = 0; gr < 2; gr++) {
/*  374 */         for (ch = 0; ch < this.channels; ch++) {
/*  375 */           this.si.ch[ch].gr[gr].part2_3_length = this.stream.get_bits(12);
/*  376 */           this.si.ch[ch].gr[gr].big_values = this.stream.get_bits(9);
/*  377 */           this.si.ch[ch].gr[gr].global_gain = this.stream.get_bits(8);
/*  378 */           this.si.ch[ch].gr[gr].scalefac_compress = this.stream.get_bits(4);
/*  379 */           this.si.ch[ch].gr[gr].window_switching_flag = this.stream.get_bits(1);
/*  380 */           if (this.si.ch[ch].gr[gr].window_switching_flag != 0) {
/*  381 */             this.si.ch[ch].gr[gr].block_type = this.stream.get_bits(2);
/*  382 */             this.si.ch[ch].gr[gr].mixed_block_flag = this.stream.get_bits(1);
/*      */ 
/*  384 */             this.si.ch[ch].gr[gr].table_select[0] = this.stream.get_bits(5);
/*  385 */             this.si.ch[ch].gr[gr].table_select[1] = this.stream.get_bits(5);
/*      */ 
/*  387 */             this.si.ch[ch].gr[gr].subblock_gain[0] = this.stream.get_bits(3);
/*  388 */             this.si.ch[ch].gr[gr].subblock_gain[1] = this.stream.get_bits(3);
/*  389 */             this.si.ch[ch].gr[gr].subblock_gain[2] = this.stream.get_bits(3);
/*      */ 
/*  393 */             if (this.si.ch[ch].gr[gr].block_type == 0)
/*      */             {
/*  395 */               return false;
/*  396 */             }if ((this.si.ch[ch].gr[gr].block_type == 2) && 
/*  397 */               (this.si.ch[ch].gr[gr].mixed_block_flag == 0))
/*  398 */               this.si.ch[ch].gr[gr].region0_count = 8;
/*      */             else {
/*  400 */               this.si.ch[ch].gr[gr].region0_count = 7;
/*      */             }
/*  402 */             this.si.ch[ch].gr[gr].region1_count = 
/*  403 */               (20 - 
/*  403 */               this.si.ch[ch].gr[gr].region0_count);
/*      */           } else {
/*  405 */             this.si.ch[ch].gr[gr].table_select[0] = this.stream.get_bits(5);
/*  406 */             this.si.ch[ch].gr[gr].table_select[1] = this.stream.get_bits(5);
/*  407 */             this.si.ch[ch].gr[gr].table_select[2] = this.stream.get_bits(5);
/*  408 */             this.si.ch[ch].gr[gr].region0_count = this.stream.get_bits(4);
/*  409 */             this.si.ch[ch].gr[gr].region1_count = this.stream.get_bits(3);
/*  410 */             this.si.ch[ch].gr[gr].block_type = 0;
/*      */           }
/*  412 */           this.si.ch[ch].gr[gr].preflag = this.stream.get_bits(1);
/*  413 */           this.si.ch[ch].gr[gr].scalefac_scale = this.stream.get_bits(1);
/*  414 */           this.si.ch[ch].gr[gr].count1table_select = this.stream.get_bits(1);
/*      */         }
/*      */       }
/*      */     }
/*      */     else
/*      */     {
/*  420 */       this.si.main_data_begin = this.stream.get_bits(8);
/*  421 */       if (this.channels == 1)
/*  422 */         this.si.private_bits = this.stream.get_bits(1);
/*  423 */       else this.si.private_bits = this.stream.get_bits(2);
/*      */ 
/*  425 */       for (int ch = 0; ch < this.channels; ch++)
/*      */       {
/*  427 */         this.si.ch[ch].gr[0].part2_3_length = this.stream.get_bits(12);
/*  428 */         this.si.ch[ch].gr[0].big_values = this.stream.get_bits(9);
/*  429 */         this.si.ch[ch].gr[0].global_gain = this.stream.get_bits(8);
/*  430 */         this.si.ch[ch].gr[0].scalefac_compress = this.stream.get_bits(9);
/*  431 */         this.si.ch[ch].gr[0].window_switching_flag = this.stream.get_bits(1);
/*      */ 
/*  433 */         if (this.si.ch[ch].gr[0].window_switching_flag != 0)
/*      */         {
/*  435 */           this.si.ch[ch].gr[0].block_type = this.stream.get_bits(2);
/*  436 */           this.si.ch[ch].gr[0].mixed_block_flag = this.stream.get_bits(1);
/*  437 */           this.si.ch[ch].gr[0].table_select[0] = this.stream.get_bits(5);
/*  438 */           this.si.ch[ch].gr[0].table_select[1] = this.stream.get_bits(5);
/*      */ 
/*  440 */           this.si.ch[ch].gr[0].subblock_gain[0] = this.stream.get_bits(3);
/*  441 */           this.si.ch[ch].gr[0].subblock_gain[1] = this.stream.get_bits(3);
/*  442 */           this.si.ch[ch].gr[0].subblock_gain[2] = this.stream.get_bits(3);
/*      */ 
/*  446 */           if (this.si.ch[ch].gr[0].block_type == 0)
/*      */           {
/*  448 */             return false;
/*  449 */           }if ((this.si.ch[ch].gr[0].block_type == 2) && 
/*  450 */             (this.si.ch[ch].gr[0].mixed_block_flag == 0)) {
/*  451 */             this.si.ch[ch].gr[0].region0_count = 8;
/*      */           } else {
/*  453 */             this.si.ch[ch].gr[0].region0_count = 7;
/*  454 */             this.si.ch[ch].gr[0].region1_count = 
/*  455 */               (20 - 
/*  455 */               this.si.ch[ch].gr[0].region0_count);
/*      */           }
/*      */         }
/*      */         else {
/*  459 */           this.si.ch[ch].gr[0].table_select[0] = this.stream.get_bits(5);
/*  460 */           this.si.ch[ch].gr[0].table_select[1] = this.stream.get_bits(5);
/*  461 */           this.si.ch[ch].gr[0].table_select[2] = this.stream.get_bits(5);
/*  462 */           this.si.ch[ch].gr[0].region0_count = this.stream.get_bits(4);
/*  463 */           this.si.ch[ch].gr[0].region1_count = this.stream.get_bits(3);
/*  464 */           this.si.ch[ch].gr[0].block_type = 0;
/*      */         }
/*      */ 
/*  467 */         this.si.ch[ch].gr[0].scalefac_scale = this.stream.get_bits(1);
/*  468 */         this.si.ch[ch].gr[0].count1table_select = this.stream.get_bits(1);
/*      */       }
/*      */     }
/*  471 */     return true;
/*      */   }
/*      */ 
/*      */   private void get_scale_factors(int ch, int gr)
/*      */   {
/*  480 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*  481 */     int scale_comp = gr_info.scalefac_compress;
/*  482 */     int length0 = slen[0][scale_comp];
/*  483 */     int length1 = slen[1][scale_comp];
/*      */ 
/*  485 */     if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2)) {
/*  486 */       if (gr_info.mixed_block_flag != 0) {
/*  487 */         for (int sfb = 0; sfb < 8; sfb++)
/*  488 */           this.scalefac[ch].l[sfb] = this.br
/*  489 */             .hgetbits(slen[0][gr_info.scalefac_compress]);
/*  490 */         for (sfb = 3; sfb < 6; sfb++)
/*  491 */           for (int window = 0; window < 3; window++)
/*  492 */             this.scalefac[ch].s[window][sfb] = this.br
/*  493 */               .hgetbits(slen[0][gr_info.scalefac_compress]);
/*  494 */         for (sfb = 6; sfb < 12; sfb++)
/*  495 */           for (int window = 0; window < 3; window++)
/*  496 */             this.scalefac[ch].s[window][sfb] = this.br
/*  497 */               .hgetbits(slen[1][gr_info.scalefac_compress]);
/*  498 */         sfb = 12; for (int window = 0; window < 3; window++)
/*  499 */           this.scalefac[ch].s[window][sfb] = 0;
/*      */       }
/*      */       else
/*      */       {
/*  503 */         this.scalefac[ch].s[0][0] = this.br.hgetbits(length0);
/*  504 */         this.scalefac[ch].s[1][0] = this.br.hgetbits(length0);
/*  505 */         this.scalefac[ch].s[2][0] = this.br.hgetbits(length0);
/*  506 */         this.scalefac[ch].s[0][1] = this.br.hgetbits(length0);
/*  507 */         this.scalefac[ch].s[1][1] = this.br.hgetbits(length0);
/*  508 */         this.scalefac[ch].s[2][1] = this.br.hgetbits(length0);
/*  509 */         this.scalefac[ch].s[0][2] = this.br.hgetbits(length0);
/*  510 */         this.scalefac[ch].s[1][2] = this.br.hgetbits(length0);
/*  511 */         this.scalefac[ch].s[2][2] = this.br.hgetbits(length0);
/*  512 */         this.scalefac[ch].s[0][3] = this.br.hgetbits(length0);
/*  513 */         this.scalefac[ch].s[1][3] = this.br.hgetbits(length0);
/*  514 */         this.scalefac[ch].s[2][3] = this.br.hgetbits(length0);
/*  515 */         this.scalefac[ch].s[0][4] = this.br.hgetbits(length0);
/*  516 */         this.scalefac[ch].s[1][4] = this.br.hgetbits(length0);
/*  517 */         this.scalefac[ch].s[2][4] = this.br.hgetbits(length0);
/*  518 */         this.scalefac[ch].s[0][5] = this.br.hgetbits(length0);
/*  519 */         this.scalefac[ch].s[1][5] = this.br.hgetbits(length0);
/*  520 */         this.scalefac[ch].s[2][5] = this.br.hgetbits(length0);
/*  521 */         this.scalefac[ch].s[0][6] = this.br.hgetbits(length1);
/*  522 */         this.scalefac[ch].s[1][6] = this.br.hgetbits(length1);
/*  523 */         this.scalefac[ch].s[2][6] = this.br.hgetbits(length1);
/*  524 */         this.scalefac[ch].s[0][7] = this.br.hgetbits(length1);
/*  525 */         this.scalefac[ch].s[1][7] = this.br.hgetbits(length1);
/*  526 */         this.scalefac[ch].s[2][7] = this.br.hgetbits(length1);
/*  527 */         this.scalefac[ch].s[0][8] = this.br.hgetbits(length1);
/*  528 */         this.scalefac[ch].s[1][8] = this.br.hgetbits(length1);
/*  529 */         this.scalefac[ch].s[2][8] = this.br.hgetbits(length1);
/*  530 */         this.scalefac[ch].s[0][9] = this.br.hgetbits(length1);
/*  531 */         this.scalefac[ch].s[1][9] = this.br.hgetbits(length1);
/*  532 */         this.scalefac[ch].s[2][9] = this.br.hgetbits(length1);
/*  533 */         this.scalefac[ch].s[0][10] = this.br.hgetbits(length1);
/*  534 */         this.scalefac[ch].s[1][10] = this.br.hgetbits(length1);
/*  535 */         this.scalefac[ch].s[2][10] = this.br.hgetbits(length1);
/*  536 */         this.scalefac[ch].s[0][11] = this.br.hgetbits(length1);
/*  537 */         this.scalefac[ch].s[1][11] = this.br.hgetbits(length1);
/*  538 */         this.scalefac[ch].s[2][11] = this.br.hgetbits(length1);
/*  539 */         this.scalefac[ch].s[0][12] = 0;
/*  540 */         this.scalefac[ch].s[1][12] = 0;
/*  541 */         this.scalefac[ch].s[2][12] = 0;
/*      */       }
/*      */     }
/*      */     else
/*      */     {
/*  546 */       if ((this.si.ch[ch].scfsi[0] == 0) || (gr == 0)) {
/*  547 */         this.scalefac[ch].l[0] = this.br.hgetbits(length0);
/*  548 */         this.scalefac[ch].l[1] = this.br.hgetbits(length0);
/*  549 */         this.scalefac[ch].l[2] = this.br.hgetbits(length0);
/*  550 */         this.scalefac[ch].l[3] = this.br.hgetbits(length0);
/*  551 */         this.scalefac[ch].l[4] = this.br.hgetbits(length0);
/*  552 */         this.scalefac[ch].l[5] = this.br.hgetbits(length0);
/*      */       }
/*  554 */       if ((this.si.ch[ch].scfsi[1] == 0) || (gr == 0)) {
/*  555 */         this.scalefac[ch].l[6] = this.br.hgetbits(length0);
/*  556 */         this.scalefac[ch].l[7] = this.br.hgetbits(length0);
/*  557 */         this.scalefac[ch].l[8] = this.br.hgetbits(length0);
/*  558 */         this.scalefac[ch].l[9] = this.br.hgetbits(length0);
/*  559 */         this.scalefac[ch].l[10] = this.br.hgetbits(length0);
/*      */       }
/*  561 */       if ((this.si.ch[ch].scfsi[2] == 0) || (gr == 0)) {
/*  562 */         this.scalefac[ch].l[11] = this.br.hgetbits(length1);
/*  563 */         this.scalefac[ch].l[12] = this.br.hgetbits(length1);
/*  564 */         this.scalefac[ch].l[13] = this.br.hgetbits(length1);
/*  565 */         this.scalefac[ch].l[14] = this.br.hgetbits(length1);
/*  566 */         this.scalefac[ch].l[15] = this.br.hgetbits(length1);
/*      */       }
/*  568 */       if ((this.si.ch[ch].scfsi[3] == 0) || (gr == 0)) {
/*  569 */         this.scalefac[ch].l[16] = this.br.hgetbits(length1);
/*  570 */         this.scalefac[ch].l[17] = this.br.hgetbits(length1);
/*  571 */         this.scalefac[ch].l[18] = this.br.hgetbits(length1);
/*  572 */         this.scalefac[ch].l[19] = this.br.hgetbits(length1);
/*  573 */         this.scalefac[ch].l[20] = this.br.hgetbits(length1);
/*      */       }
/*      */ 
/*  576 */       this.scalefac[ch].l[21] = 0;
/*  577 */       this.scalefac[ch].l[22] = 0;
/*      */     }
/*      */   }
/*      */ 
/*      */   private void get_LSF_scale_data(int ch, int gr)
/*      */   {
/*  592 */     int mode_ext = this.header.mode_extension();
/*      */ 
/*  595 */     int blocknumber = 0;
/*      */ 
/*  597 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*      */ 
/*  599 */     int scalefac_comp = gr_info.scalefac_compress;
/*      */     int blocktypenumber;
/*      */     int blocktypenumber;
/*  601 */     if (gr_info.block_type == 2)
/*      */     {
/*      */       int blocktypenumber;
/*  602 */       if (gr_info.mixed_block_flag == 0) {
/*  603 */         blocktypenumber = 1;
/*      */       }
/*      */       else
/*      */       {
/*      */         int blocktypenumber;
/*  604 */         if (gr_info.mixed_block_flag == 1)
/*  605 */           blocktypenumber = 2;
/*      */         else
/*  607 */           blocktypenumber = 0; 
/*      */       }
/*      */     } else {
/*  609 */       blocktypenumber = 0;
/*      */     }
/*      */ 
/*  612 */     if (((mode_ext != 1) && (mode_ext != 3)) || (ch != 1))
/*      */     {
/*  614 */       if (scalefac_comp < 400)
/*      */       {
/*  616 */         this.new_slen[0] = ((scalefac_comp >>> 4) / 5);
/*  617 */         this.new_slen[1] = ((scalefac_comp >>> 4) % 5);
/*  618 */         this.new_slen[2] = ((scalefac_comp & 0xF) >>> 2);
/*  619 */         this.new_slen[3] = (scalefac_comp & 0x3);
/*  620 */         this.si.ch[ch].gr[gr].preflag = 0;
/*  621 */         blocknumber = 0;
/*      */       }
/*  623 */       else if (scalefac_comp < 500)
/*      */       {
/*  625 */         this.new_slen[0] = ((scalefac_comp - 400 >>> 2) / 5);
/*  626 */         this.new_slen[1] = ((scalefac_comp - 400 >>> 2) % 5);
/*  627 */         this.new_slen[2] = (scalefac_comp - 400 & 0x3);
/*  628 */         this.new_slen[3] = 0;
/*  629 */         this.si.ch[ch].gr[gr].preflag = 0;
/*  630 */         blocknumber = 1;
/*      */       }
/*  632 */       else if (scalefac_comp < 512)
/*      */       {
/*  634 */         this.new_slen[0] = ((scalefac_comp - 500) / 3);
/*  635 */         this.new_slen[1] = ((scalefac_comp - 500) % 3);
/*  636 */         this.new_slen[2] = 0;
/*  637 */         this.new_slen[3] = 0;
/*  638 */         this.si.ch[ch].gr[gr].preflag = 1;
/*  639 */         blocknumber = 2;
/*      */       }
/*      */     }
/*      */ 
/*  643 */     if (((mode_ext == 1) || (mode_ext == 3)) && (ch == 1))
/*      */     {
/*  645 */       int int_scalefac_comp = scalefac_comp >>> 1;
/*      */ 
/*  647 */       if (int_scalefac_comp < 180)
/*      */       {
/*  649 */         this.new_slen[0] = (int_scalefac_comp / 36);
/*  650 */         this.new_slen[1] = (int_scalefac_comp % 36 / 6);
/*  651 */         this.new_slen[2] = (int_scalefac_comp % 36 % 6);
/*  652 */         this.new_slen[3] = 0;
/*  653 */         this.si.ch[ch].gr[gr].preflag = 0;
/*  654 */         blocknumber = 3;
/*  655 */       } else if (int_scalefac_comp < 244) {
/*  656 */         this.new_slen[0] = ((int_scalefac_comp - 180 & 0x3F) >>> 4);
/*  657 */         this.new_slen[1] = ((int_scalefac_comp - 180 & 0xF) >>> 2);
/*  658 */         this.new_slen[2] = (int_scalefac_comp - 180 & 0x3);
/*  659 */         this.new_slen[3] = 0;
/*  660 */         this.si.ch[ch].gr[gr].preflag = 0;
/*  661 */         blocknumber = 4;
/*  662 */       } else if (int_scalefac_comp < 255) {
/*  663 */         this.new_slen[0] = ((int_scalefac_comp - 244) / 3);
/*  664 */         this.new_slen[1] = ((int_scalefac_comp - 244) % 3);
/*  665 */         this.new_slen[2] = 0;
/*  666 */         this.new_slen[3] = 0;
/*  667 */         this.si.ch[ch].gr[gr].preflag = 0;
/*  668 */         blocknumber = 5;
/*      */       }
/*      */     }
/*      */ 
/*  672 */     for (int x = 0; x < 45; x++) {
/*  673 */       this.scalefac_buffer[x] = 0;
/*      */     }
/*  675 */     int m = 0;
/*  676 */     for (int i = 0; i < 4; i++)
/*  677 */       for (int j = 0; j < nr_of_sfb_block[blocknumber][blocktypenumber][i]; )
/*      */       {
/*  680 */         this.scalefac_buffer[m] = (this.new_slen[i] == 0 ? 0 : 
/*  681 */           this.br.hgetbits(this.new_slen[i]));
/*  682 */         m++;
/*      */ 
/*  678 */         j++;
/*      */       }
/*      */   }
/*      */ 
/*      */   private void get_LSF_scale_factors(int ch, int gr)
/*      */   {
/*  693 */     int m = 0;
/*      */ 
/*  695 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*      */ 
/*  697 */     get_LSF_scale_data(ch, gr);
/*      */ 
/*  699 */     if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2)) {
/*  700 */       if (gr_info.mixed_block_flag != 0) {
/*  701 */         for (int sfb = 0; sfb < 8; sfb++)
/*      */         {
/*  703 */           this.scalefac[ch].l[sfb] = this.scalefac_buffer[m];
/*  704 */           m++;
/*      */         }
/*  706 */         for (sfb = 3; sfb < 12; sfb++) {
/*  707 */           for (int window = 0; window < 3; window++)
/*      */           {
/*  709 */             this.scalefac[ch].s[window][sfb] = this.scalefac_buffer[m];
/*  710 */             m++;
/*      */           }
/*      */         }
/*  713 */         for (int window = 0; window < 3; window++)
/*  714 */           this.scalefac[ch].s[window][12] = 0;
/*      */       }
/*      */       else
/*      */       {
/*  718 */         for (int sfb = 0; sfb < 12; sfb++) {
/*  719 */           for (int window = 0; window < 3; window++)
/*      */           {
/*  721 */             this.scalefac[ch].s[window][sfb] = this.scalefac_buffer[m];
/*  722 */             m++;
/*      */           }
/*      */         }
/*      */ 
/*  726 */         for (int window = 0; window < 3; window++)
/*  727 */           this.scalefac[ch].s[window][12] = 0;
/*      */       }
/*      */     }
/*      */     else {
/*  731 */       for (int sfb = 0; sfb < 21; sfb++) {
/*  732 */         this.scalefac[ch].l[sfb] = this.scalefac_buffer[m];
/*  733 */         m++;
/*      */       }
/*  735 */       this.scalefac[ch].l[21] = 0;
/*  736 */       this.scalefac[ch].l[22] = 0;
/*      */     }
/*      */   }
/*      */ 
/*      */   private void huffman_decode(int ch, int gr)
/*      */   {
/*  749 */     this.x[0] = 0;
/*  750 */     this.y[0] = 0;
/*  751 */     this.v[0] = 0;
/*  752 */     this.w[0] = 0;
/*      */ 
/*  754 */     int part2_3_end = this.part2_start + this.si.ch[ch].gr[gr].part2_3_length;
/*      */     int region2Start;
/*      */     int region1Start;
/*      */     int region2Start;
/*  766 */     if ((this.si.ch[ch].gr[gr].window_switching_flag != 0) && 
/*  767 */       (this.si.ch[ch].gr[gr].block_type == 2))
/*      */     {
/*  771 */       int region1Start = this.sfreq == 8 ? 72 : 36;
/*  772 */       region2Start = 576;
/*      */     }
/*      */     else
/*      */     {
/*  776 */       int buf = this.si.ch[ch].gr[gr].region0_count + 1;
/*  777 */       int buf1 = buf + this.si.ch[ch].gr[gr].region1_count + 1;
/*      */ 
/*  779 */       if (buf1 > this.sfBandIndex[this.sfreq].l.length - 1) buf1 = this.sfBandIndex[this.sfreq].l.length - 1;
/*      */ 
/*  781 */       region1Start = this.sfBandIndex[this.sfreq].l[buf];
/*  782 */       region2Start = this.sfBandIndex[this.sfreq].l[buf1];
/*      */     }
/*      */ 
/*  785 */     int index = 0;
/*      */ 
/*  787 */     for (int i = 0; i < this.si.ch[ch].gr[gr].big_values << 1; i += 2)
/*      */     {
/*  788 */       huffcodetab h;
/*      */       huffcodetab h;
/*  788 */       if (i < region1Start) { h = huffcodetab.ht[this.si.ch[ch].gr[gr].table_select[0]];
/*      */       }
/*      */       else
/*      */       {
/*  789 */         huffcodetab h;
/*  789 */         if (i < region2Start) h = huffcodetab.ht[this.si.ch[ch].gr[gr].table_select[1]]; else
/*  790 */           h = huffcodetab.ht[this.si.ch[ch].gr[gr].table_select[2]];
/*      */       }
/*  792 */       huffcodetab.huffman_decoder(h, this.x, this.y, this.v, this.w, this.br);
/*      */ 
/*  795 */       this.is_1d[(index++)] = this.x[0];
/*  796 */       this.is_1d[(index++)] = this.y[0];
/*      */ 
/*  798 */       this.CheckSumHuff = (this.CheckSumHuff + this.x[0] + this.y[0]);
/*      */     }
/*      */ 
/*  803 */     huffcodetab h = huffcodetab.ht[(this.si.ch[ch].gr[gr].count1table_select + 32)];
/*  804 */     int num_bits = this.br.hsstell();
/*      */ 
/*  806 */     while ((num_bits < part2_3_end) && (index < 576))
/*      */     {
/*  808 */       huffcodetab.huffman_decoder(h, this.x, this.y, this.v, this.w, this.br);
/*      */ 
/*  810 */       this.is_1d[(index++)] = this.v[0];
/*  811 */       this.is_1d[(index++)] = this.w[0];
/*  812 */       this.is_1d[(index++)] = this.x[0];
/*  813 */       this.is_1d[(index++)] = this.y[0];
/*  814 */       this.CheckSumHuff = (this.CheckSumHuff + this.v[0] + this.w[0] + this.x[0] + this.y[0]);
/*      */ 
/*  817 */       num_bits = this.br.hsstell();
/*      */     }
/*      */ 
/*  820 */     if (num_bits > part2_3_end) {
/*  821 */       this.br.rewindNbits(num_bits - part2_3_end);
/*  822 */       index -= 4;
/*      */     }
/*      */ 
/*  825 */     num_bits = this.br.hsstell();
/*      */ 
/*  828 */     if (num_bits < part2_3_end) {
/*  829 */       this.br.hgetbits(part2_3_end - num_bits);
/*      */     }
/*      */ 
/*  833 */     if (index < 576)
/*  834 */       this.nonzero[ch] = index;
/*      */     else {
/*  836 */       this.nonzero[ch] = 576;
/*      */     }
/*  838 */     if (index < 0) index = 0;
/*      */ 
/*  841 */     for (; index < 576; index++)
/*  842 */       this.is_1d[index] = 0;
/*      */   }
/*      */ 
/*      */   private void i_stereo_k_values(int is_pos, int io_type, int i)
/*      */   {
/*  850 */     if (is_pos == 0) {
/*  851 */       this.k[0][i] = 1.0F;
/*  852 */       this.k[1][i] = 1.0F;
/*  853 */     } else if ((is_pos & 0x1) != 0) {
/*  854 */       this.k[0][i] = io[io_type][(is_pos + 1 >>> 1)];
/*  855 */       this.k[1][i] = 1.0F;
/*      */     } else {
/*  857 */       this.k[0][i] = 1.0F;
/*  858 */       this.k[1][i] = io[io_type][(is_pos >>> 1)];
/*      */     }
/*      */   }
/*      */ 
/*      */   private void dequantize_sample(float[][] xr, int ch, int gr)
/*      */   {
/*  867 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*  868 */     int cb = 0;
/*      */ 
/*  870 */     int cb_begin = 0;
/*  871 */     int cb_width = 0;
/*  872 */     int index = 0;
/*      */ 
/*  874 */     float[][] xr_1d = xr;
/*      */     int next_cb_boundary;
/*  878 */     if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2))
/*      */     {
/*      */       int next_cb_boundary;
/*  879 */       if (gr_info.mixed_block_flag != 0) {
/*  880 */         next_cb_boundary = this.sfBandIndex[this.sfreq].l[1];
/*      */       } else {
/*  882 */         cb_width = this.sfBandIndex[this.sfreq].s[1];
/*  883 */         int next_cb_boundary = (cb_width << 2) - cb_width;
/*  884 */         cb_begin = 0;
/*      */       }
/*      */     } else {
/*  887 */       next_cb_boundary = this.sfBandIndex[this.sfreq].l[1];
/*      */     }
/*      */ 
/*  892 */     float g_gain = (float)Math.pow(2.0D, 0.25D * (gr_info.global_gain - 210.0D));
/*      */ 
/*  894 */     for (int j = 0; j < this.nonzero[ch]; j++)
/*      */     {
/*  897 */       int reste = j % 18;
/*  898 */       int quotien = (j - reste) / 18;
/*  899 */       if (this.is_1d[j] == 0) { xr_1d[quotien][reste] = 0.0F;
/*      */       } else
/*      */       {
/*  902 */         int abv = this.is_1d[j];
/*      */ 
/*  904 */         if (abv < t_43.length)
/*      */         {
/*  906 */           if (this.is_1d[j] > 0) xr_1d[quotien][reste] = (g_gain * t_43[abv]);
/*  909 */           else if (-abv < t_43.length) xr_1d[quotien][reste] = (-g_gain * t_43[(-abv)]); else {
/*  910 */             xr_1d[quotien][reste] = (-g_gain * (float)Math.pow(-abv, 1.333333333333333D));
/*      */           }
/*      */ 
/*      */         }
/*  915 */         else if (this.is_1d[j] > 0) xr_1d[quotien][reste] = (g_gain * (float)Math.pow(abv, 1.333333333333333D)); else {
/*  916 */           xr_1d[quotien][reste] = (-g_gain * (float)Math.pow(-abv, 1.333333333333333D));
/*      */         }
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  922 */     for (j = 0; j < this.nonzero[ch]; j++)
/*      */     {
/*  925 */       int reste = j % 18;
/*  926 */       int quotien = (j - reste) / 18;
/*      */ 
/*  928 */       if (index == next_cb_boundary) {
/*  929 */         if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2)) {
/*  930 */           if (gr_info.mixed_block_flag != 0)
/*      */           {
/*  932 */             if (index == this.sfBandIndex[this.sfreq].l[8]) {
/*  933 */               next_cb_boundary = this.sfBandIndex[this.sfreq].s[4];
/*  934 */               next_cb_boundary = (next_cb_boundary << 2) - 
/*  935 */                 next_cb_boundary;
/*  936 */               cb = 3;
/*  937 */               cb_width = this.sfBandIndex[this.sfreq].s[4] - 
/*  938 */                 this.sfBandIndex[this.sfreq].s[3];
/*      */ 
/*  940 */               cb_begin = this.sfBandIndex[this.sfreq].s[3];
/*  941 */               cb_begin = (cb_begin << 2) - cb_begin;
/*      */             }
/*  943 */             else if (index < this.sfBandIndex[this.sfreq].l[8])
/*      */             {
/*  945 */               cb++; next_cb_boundary = this.sfBandIndex[this.sfreq].l[(cb + 1)];
/*      */             }
/*      */             else
/*      */             {
/*  949 */               cb++; next_cb_boundary = this.sfBandIndex[this.sfreq].s[(cb + 1)];
/*  950 */               next_cb_boundary = (next_cb_boundary << 2) - 
/*  951 */                 next_cb_boundary;
/*      */ 
/*  953 */               cb_begin = this.sfBandIndex[this.sfreq].s[cb];
/*  954 */               cb_width = this.sfBandIndex[this.sfreq].s[(cb + 1)] - 
/*  955 */                 cb_begin;
/*  956 */               cb_begin = (cb_begin << 2) - cb_begin;
/*      */             }
/*      */           }
/*      */           else
/*      */           {
/*  961 */             cb++; next_cb_boundary = this.sfBandIndex[this.sfreq].s[(cb + 1)];
/*  962 */             next_cb_boundary = (next_cb_boundary << 2) - 
/*  963 */               next_cb_boundary;
/*      */ 
/*  965 */             cb_begin = this.sfBandIndex[this.sfreq].s[cb];
/*  966 */             cb_width = this.sfBandIndex[this.sfreq].s[(cb + 1)] - 
/*  967 */               cb_begin;
/*  968 */             cb_begin = (cb_begin << 2) - cb_begin;
/*      */           }
/*      */         }
/*      */         else
/*      */         {
/*  973 */           cb++; next_cb_boundary = this.sfBandIndex[this.sfreq].l[(cb + 1)];
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*  980 */       if ((gr_info.window_switching_flag != 0) && (
/*  981 */         ((gr_info.block_type == 2) && (gr_info.mixed_block_flag == 0)) || (
/*  982 */         (gr_info.block_type == 2) && (gr_info.mixed_block_flag != 0) && (j >= 36))))
/*      */       {
/*  985 */         int t_index = (index - cb_begin) / cb_width;
/*      */ 
/*  989 */         int idx = this.scalefac[ch].s[t_index][cb] << 
/*  990 */           gr_info.scalefac_scale;
/*  991 */         idx += (gr_info.subblock_gain[t_index] << 2);
/*      */ 
/*  993 */         xr_1d[quotien][reste] *= two_to_negative_half_pow[idx];
/*      */       }
/*      */       else
/*      */       {
/*  999 */         int idx = this.scalefac[ch].l[cb];
/*      */ 
/* 1001 */         if (gr_info.preflag != 0) {
/* 1002 */           idx += pretab[cb];
/*      */         }
/* 1004 */         idx <<= gr_info.scalefac_scale;
/* 1005 */         xr_1d[quotien][reste] *= two_to_negative_half_pow[idx];
/*      */       }
/* 1007 */       index++;
/*      */     }
/*      */ 
/* 1010 */     for (j = this.nonzero[ch]; j < 576; j++)
/*      */     {
/* 1013 */       int reste = j % 18;
/* 1014 */       int quotien = (j - reste) / 18;
/* 1015 */       if (reste < 0) reste = 0;
/* 1016 */       if (quotien < 0) quotien = 0;
/* 1017 */       xr_1d[quotien][reste] = 0.0F;
/*      */     }
/*      */   }
/*      */ 
/*      */   private void reorder(float[][] xr, int ch, int gr)
/*      */   {
/* 1028 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*      */ 
/* 1033 */     float[][] xr_1d = xr;
/*      */ 
/* 1035 */     if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2))
/*      */     {
/* 1037 */       for (int index = 0; index < 576; index++) {
/* 1038 */         this.out_1d[index] = 0.0F;
/*      */       }
/* 1040 */       if (gr_info.mixed_block_flag != 0)
/*      */       {
/* 1042 */         for (index = 0; index < 36; index++)
/*      */         {
/* 1045 */           int reste = index % 18;
/* 1046 */           int quotien = (index - reste) / 18;
/* 1047 */           this.out_1d[index] = xr_1d[quotien][reste];
/*      */         }
/*      */ 
/* 1055 */         for (int sfb = 3; sfb < 13; sfb++)
/*      */         {
/* 1058 */           int sfb_start = this.sfBandIndex[this.sfreq].s[sfb];
/* 1059 */           int sfb_lines = this.sfBandIndex[this.sfreq].s[(sfb + 1)] - sfb_start;
/*      */ 
/* 1061 */           int sfb_start3 = (sfb_start << 2) - sfb_start;
/*      */ 
/* 1063 */           int freq = 0; for (int freq3 = 0; freq < sfb_lines; )
/*      */           {
/* 1066 */             int src_line = sfb_start3 + freq;
/* 1067 */             int des_line = sfb_start3 + freq3;
/*      */ 
/* 1069 */             int reste = src_line % 18;
/* 1070 */             int quotien = (src_line - reste) / 18;
/*      */ 
/* 1072 */             this.out_1d[des_line] = xr_1d[quotien][reste];
/* 1073 */             src_line += sfb_lines;
/* 1074 */             des_line++;
/*      */ 
/* 1076 */             reste = src_line % 18;
/* 1077 */             quotien = (src_line - reste) / 18;
/*      */ 
/* 1079 */             this.out_1d[des_line] = xr_1d[quotien][reste];
/* 1080 */             src_line += sfb_lines;
/* 1081 */             des_line++;
/*      */ 
/* 1083 */             reste = src_line % 18;
/* 1084 */             quotien = (src_line - reste) / 18;
/*      */ 
/* 1086 */             this.out_1d[des_line] = xr_1d[quotien][reste];
/*      */ 
/* 1064 */             freq++; freq3 += 3;
/*      */           }
/*      */ 
/*      */         }
/*      */ 
/*      */       }
/*      */       else
/*      */       {
/* 1091 */         for (index = 0; index < 576; index++)
/*      */         {
/* 1093 */           int j = reorder_table[this.sfreq][index];
/* 1094 */           int reste = j % 18;
/* 1095 */           int quotien = (j - reste) / 18;
/* 1096 */           this.out_1d[index] = xr_1d[quotien][reste];
/*      */         }
/*      */       }
/*      */     }
/*      */     else {
/* 1101 */       for (int index = 0; index < 576; index++)
/*      */       {
/* 1104 */         int reste = index % 18;
/* 1105 */         int quotien = (index - reste) / 18;
/* 1106 */         this.out_1d[index] = xr_1d[quotien][reste];
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private void stereo(int gr)
/*      */   {
/* 1122 */     if (this.channels == 1)
/*      */     {
/* 1124 */       for (int sb = 0; sb < 32; sb++)
/* 1125 */         for (int ss = 0; ss < 18; ss += 3) {
/* 1126 */           this.lr[0][sb][ss] = this.ro[0][sb][ss];
/* 1127 */           this.lr[0][sb][(ss + 1)] = this.ro[0][sb][(ss + 1)];
/* 1128 */           this.lr[0][sb][(ss + 2)] = this.ro[0][sb][(ss + 2)];
/*      */         }
/*      */     }
/*      */     else
/*      */     {
/* 1133 */       gr_info_s gr_info = this.si.ch[0].gr[gr];
/* 1134 */       int mode_ext = this.header.mode_extension();
/*      */ 
/* 1139 */       boolean ms_stereo = (this.header.mode() == 1) && ((mode_ext & 0x2) != 0);
/* 1140 */       boolean i_stereo = (this.header.mode() == 1) && ((mode_ext & 0x1) != 0);
/* 1141 */       boolean lsf = (this.header.version() == 0) || (this.header.version() == 2);
/*      */ 
/* 1143 */       int io_type = gr_info.scalefac_compress & 0x1;
/*      */ 
/* 1147 */       for (int i = 0; i < 576; i++)
/*      */       {
/* 1149 */         this.is_pos[i] = 7;
/*      */ 
/* 1151 */         this.is_ratio[i] = 0.0F;
/*      */       }
/*      */ 
/* 1154 */       if (i_stereo) {
/* 1155 */         if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2)) {
/* 1156 */           if (gr_info.mixed_block_flag != 0)
/*      */           {
/* 1158 */             int max_sfb = 0;
/*      */ 
/* 1160 */             for (int j = 0; j < 3; j++)
/*      */             {
/* 1162 */               int sfbcnt = 2;
/* 1163 */               for (int sfb = 12; sfb >= 3; sfb--) {
/* 1164 */                 i = this.sfBandIndex[this.sfreq].s[sfb];
/* 1165 */                 int lines = this.sfBandIndex[this.sfreq].s[(sfb + 1)] - i;
/* 1166 */                 i = (i << 2) - i + (j + 1) * lines - 1;
/*      */ 
/* 1168 */                 while (lines > 0) {
/* 1169 */                   if (this.ro[1][(i / 18)][(i % 18)] != 0.0F)
/*      */                   {
/* 1173 */                     sfbcnt = sfb;
/* 1174 */                     sfb = -10;
/* 1175 */                     lines = -10;
/*      */                   }
/*      */ 
/* 1178 */                   lines--;
/* 1179 */                   i--;
/*      */                 }
/*      */ 
/*      */               }
/*      */ 
/* 1184 */               sfb = sfbcnt + 1;
/*      */ 
/* 1186 */               if (sfb > max_sfb) {
/* 1187 */                 max_sfb = sfb;
/*      */               }
/* 1189 */               while (sfb < 12) {
/* 1190 */                 int temp = this.sfBandIndex[this.sfreq].s[sfb];
/* 1191 */                 int sb = this.sfBandIndex[this.sfreq].s[(sfb + 1)] - temp;
/* 1192 */                 i = (temp << 2) - temp + j * sb;
/*      */ 
/* 1194 */                 for (; sb > 0; sb--) {
/* 1195 */                   this.is_pos[i] = this.scalefac[1].s[j][sfb];
/* 1196 */                   if (this.is_pos[i] != 7) {
/* 1197 */                     if (lsf)
/* 1198 */                       i_stereo_k_values(this.is_pos[i], io_type, i);
/*      */                     else
/* 1200 */                       this.is_ratio[i] = TAN12[this.is_pos[i]];
/*      */                   }
/* 1202 */                   i++;
/*      */                 }
/* 1204 */                 sfb++;
/*      */               }
/* 1206 */               sfb = this.sfBandIndex[this.sfreq].s[10];
/* 1207 */               int sb = this.sfBandIndex[this.sfreq].s[11] - sfb;
/* 1208 */               sfb = (sfb << 2) - sfb + j * sb;
/* 1209 */               int temp = this.sfBandIndex[this.sfreq].s[11];
/* 1210 */               sb = this.sfBandIndex[this.sfreq].s[12] - temp;
/* 1211 */               i = (temp << 2) - temp + j * sb;
/*      */ 
/* 1213 */               for (; sb > 0; sb--) {
/* 1214 */                 this.is_pos[i] = this.is_pos[sfb];
/*      */ 
/* 1216 */                 if (lsf) {
/* 1217 */                   this.k[0][i] = this.k[0][sfb];
/* 1218 */                   this.k[1][i] = this.k[1][sfb];
/*      */                 } else {
/* 1220 */                   this.is_ratio[i] = this.is_ratio[sfb];
/*      */                 }
/* 1222 */                 i++;
/*      */               }
/*      */             }
/* 1225 */             if (max_sfb <= 3) {
/* 1226 */               i = 2;
/* 1227 */               int ss = 17;
/* 1228 */               int sb = -1;
/* 1229 */               while (i >= 0) {
/* 1230 */                 if (this.ro[1][i][ss] != 0.0F) {
/* 1231 */                   sb = (i << 4) + (i << 1) + ss;
/* 1232 */                   i = -1;
/*      */                 } else {
/* 1234 */                   ss--;
/* 1235 */                   if (ss < 0) {
/* 1236 */                     i--;
/* 1237 */                     ss = 17;
/*      */                   }
/*      */                 }
/*      */               }
/* 1241 */               i = 0;
/* 1242 */               while (this.sfBandIndex[this.sfreq].l[i] <= sb)
/* 1243 */                 i++;
/* 1244 */               int sfb = i;
/* 1245 */               i = this.sfBandIndex[this.sfreq].l[i];
/* 1246 */               for (; sfb < 8; sfb++) {
/* 1247 */                 sb = this.sfBandIndex[this.sfreq].l[(sfb + 1)] - this.sfBandIndex[this.sfreq].l[sfb];
/* 1248 */                 for (; sb > 0; sb--) {
/* 1249 */                   this.is_pos[i] = this.scalefac[1].l[sfb];
/* 1250 */                   if (this.is_pos[i] != 7)
/* 1251 */                     if (lsf)
/* 1252 */                       i_stereo_k_values(this.is_pos[i], io_type, i);
/*      */                     else
/* 1254 */                       this.is_ratio[i] = TAN12[this.is_pos[i]];
/* 1255 */                   i++;
/*      */                 }
/*      */               }
/*      */             }
/*      */           } else {
/* 1260 */             for (int j = 0; j < 3; j++)
/*      */             {
/* 1262 */               int sfbcnt = -1;
/* 1263 */               for (int sfb = 12; sfb >= 0; sfb--)
/*      */               {
/* 1265 */                 int temp = this.sfBandIndex[this.sfreq].s[sfb];
/* 1266 */                 int lines = this.sfBandIndex[this.sfreq].s[(sfb + 1)] - temp;
/* 1267 */                 i = (temp << 2) - temp + (j + 1) * lines - 1;
/*      */ 
/* 1269 */                 while (lines > 0) {
/* 1270 */                   if (this.ro[1][(i / 18)][(i % 18)] != 0.0F)
/*      */                   {
/* 1274 */                     sfbcnt = sfb;
/* 1275 */                     sfb = -10;
/* 1276 */                     lines = -10;
/*      */                   }
/* 1278 */                   lines--;
/* 1279 */                   i--;
/*      */                 }
/*      */               }
/*      */ 
/* 1283 */               sfb = sfbcnt + 1;
/* 1284 */               while (sfb < 12) {
/* 1285 */                 int temp = this.sfBandIndex[this.sfreq].s[sfb];
/* 1286 */                 int sb = this.sfBandIndex[this.sfreq].s[(sfb + 1)] - temp;
/* 1287 */                 i = (temp << 2) - temp + j * sb;
/* 1288 */                 for (; sb > 0; sb--) {
/* 1289 */                   this.is_pos[i] = this.scalefac[1].s[j][sfb];
/* 1290 */                   if (this.is_pos[i] != 7)
/* 1291 */                     if (lsf)
/* 1292 */                       i_stereo_k_values(this.is_pos[i], io_type, i);
/*      */                     else
/* 1294 */                       this.is_ratio[i] = TAN12[this.is_pos[i]];
/* 1295 */                   i++;
/*      */                 }
/* 1297 */                 sfb++;
/*      */               }
/*      */ 
/* 1300 */               int temp = this.sfBandIndex[this.sfreq].s[10];
/* 1301 */               int temp2 = this.sfBandIndex[this.sfreq].s[11];
/* 1302 */               int sb = temp2 - temp;
/* 1303 */               sfb = (temp << 2) - temp + j * sb;
/* 1304 */               sb = this.sfBandIndex[this.sfreq].s[12] - temp2;
/* 1305 */               i = (temp2 << 2) - temp2 + j * sb;
/*      */ 
/* 1307 */               for (; sb > 0; sb--) {
/* 1308 */                 this.is_pos[i] = this.is_pos[sfb];
/*      */ 
/* 1310 */                 if (lsf) {
/* 1311 */                   this.k[0][i] = this.k[0][sfb];
/* 1312 */                   this.k[1][i] = this.k[1][sfb];
/*      */                 } else {
/* 1314 */                   this.is_ratio[i] = this.is_ratio[sfb];
/*      */                 }
/* 1316 */                 i++;
/*      */               }
/*      */             }
/*      */           }
/*      */         } else {
/* 1321 */           i = 31;
/* 1322 */           int ss = 17;
/* 1323 */           int sb = 0;
/* 1324 */           while (i >= 0) {
/* 1325 */             if (this.ro[1][i][ss] != 0.0F) {
/* 1326 */               sb = (i << 4) + (i << 1) + ss;
/* 1327 */               i = -1;
/*      */             } else {
/* 1329 */               ss--;
/* 1330 */               if (ss < 0) {
/* 1331 */                 i--;
/* 1332 */                 ss = 17;
/*      */               }
/*      */             }
/*      */           }
/* 1336 */           i = 0;
/* 1337 */           while (this.sfBandIndex[this.sfreq].l[i] <= sb) {
/* 1338 */             i++;
/*      */           }
/* 1340 */           int sfb = i;
/* 1341 */           i = this.sfBandIndex[this.sfreq].l[i];
/* 1342 */           for (; sfb < 21; sfb++) {
/* 1343 */             sb = this.sfBandIndex[this.sfreq].l[(sfb + 1)] - this.sfBandIndex[this.sfreq].l[sfb];
/* 1344 */             for (; sb > 0; sb--) {
/* 1345 */               this.is_pos[i] = this.scalefac[1].l[sfb];
/* 1346 */               if (this.is_pos[i] != 7)
/* 1347 */                 if (lsf)
/* 1348 */                   i_stereo_k_values(this.is_pos[i], io_type, i);
/*      */                 else
/* 1350 */                   this.is_ratio[i] = TAN12[this.is_pos[i]];
/* 1351 */               i++;
/*      */             }
/*      */           }
/* 1354 */           sfb = this.sfBandIndex[this.sfreq].l[20];
/* 1355 */           for (sb = 576 - this.sfBandIndex[this.sfreq].l[21]; (sb > 0) && (i < 576); sb--)
/*      */           {
/* 1357 */             this.is_pos[i] = this.is_pos[sfb];
/*      */ 
/* 1359 */             if (lsf) {
/* 1360 */               this.k[0][i] = this.k[0][sfb];
/* 1361 */               this.k[1][i] = this.k[1][sfb];
/*      */             } else {
/* 1363 */               this.is_ratio[i] = this.is_ratio[sfb];
/*      */             }
/* 1365 */             i++;
/*      */           }
/*      */         }
/*      */       }
/*      */ 
/* 1370 */       i = 0;
/* 1371 */       for (int sb = 0; sb < 32; sb++)
/* 1372 */         for (int ss = 0; ss < 18; ss++) {
/* 1373 */           if (this.is_pos[i] == 7) {
/* 1374 */             if (ms_stereo) {
/* 1375 */               this.lr[0][sb][ss] = ((this.ro[0][sb][ss] + this.ro[1][sb][ss]) * 0.7071068F);
/* 1376 */               this.lr[1][sb][ss] = ((this.ro[0][sb][ss] - this.ro[1][sb][ss]) * 0.7071068F);
/*      */             } else {
/* 1378 */               this.lr[0][sb][ss] = this.ro[0][sb][ss];
/* 1379 */               this.lr[1][sb][ss] = this.ro[1][sb][ss];
/*      */             }
/*      */           }
/* 1382 */           else if (i_stereo)
/*      */           {
/* 1384 */             if (lsf) {
/* 1385 */               this.lr[0][sb][ss] = (this.ro[0][sb][ss] * this.k[0][i]);
/* 1386 */               this.lr[1][sb][ss] = (this.ro[0][sb][ss] * this.k[1][i]);
/*      */             } else {
/* 1388 */               this.lr[1][sb][ss] = (this.ro[0][sb][ss] / (1.0F + this.is_ratio[i]));
/* 1389 */               this.lr[0][sb][ss] = (this.lr[1][sb][ss] * this.is_ratio[i]);
/*      */             }
/*      */ 
/*      */           }
/*      */ 
/* 1395 */           i++;
/*      */         }
/*      */     }
/*      */   }
/*      */ 
/*      */   private void antialias(int ch, int gr)
/*      */   {
/* 1408 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*      */ 
/* 1412 */     if ((gr_info.window_switching_flag != 0) && (gr_info.block_type == 2) && 
/* 1413 */       (gr_info.mixed_block_flag == 0))
/* 1414 */       return;
/*      */     int sb18lim;
/*      */     int sb18lim;
/* 1416 */     if ((gr_info.window_switching_flag != 0) && (gr_info.mixed_block_flag != 0) && 
/* 1417 */       (gr_info.block_type == 2))
/* 1418 */       sb18lim = 18;
/*      */     else {
/* 1420 */       sb18lim = 558;
/*      */     }
/*      */ 
/* 1423 */     for (int sb18 = 0; sb18 < sb18lim; sb18 += 18)
/* 1424 */       for (int ss = 0; ss < 8; ss++) {
/* 1425 */         int src_idx1 = sb18 + 17 - ss;
/* 1426 */         int src_idx2 = sb18 + 18 + ss;
/* 1427 */         float bu = this.out_1d[src_idx1];
/* 1428 */         float bd = this.out_1d[src_idx2];
/* 1429 */         this.out_1d[src_idx1] = (bu * cs[ss] - bd * ca[ss]);
/* 1430 */         this.out_1d[src_idx2] = (bd * cs[ss] + bu * ca[ss]);
/*      */       }
/*      */   }
/*      */ 
/*      */   private void hybrid(int ch, int gr)
/*      */   {
/* 1448 */     gr_info_s gr_info = this.si.ch[ch].gr[gr];
/*      */ 
/* 1453 */     for (int sb18 = 0; sb18 < 576; sb18 += 18)
/*      */     {
/* 1455 */       int bt = (gr_info.window_switching_flag != 0) && (gr_info.mixed_block_flag != 0) && 
/* 1456 */         (sb18 < 36) ? 
/* 1456 */         0 : gr_info.block_type;
/*      */ 
/* 1458 */       float[] tsOut = this.out_1d;
/*      */ 
/* 1460 */       for (int cc = 0; cc < 18; cc++) {
/* 1461 */         this.tsOutCopy[cc] = tsOut[(cc + sb18)];
/*      */       }
/* 1463 */       inv_mdct(this.tsOutCopy, this.rawout, bt);
/*      */ 
/* 1466 */       for (int cc = 0; cc < 18; cc++) {
/* 1467 */         tsOut[(cc + sb18)] = this.tsOutCopy[cc];
/*      */       }
/*      */ 
/* 1471 */       float[][] prvblk = this.prevblck;
/*      */ 
/* 1473 */       tsOut[(0 + sb18)] = (this.rawout[0] + prvblk[ch][(sb18 + 0)]);
/* 1474 */       prvblk[ch][(sb18 + 0)] = this.rawout[18];
/* 1475 */       tsOut[(1 + sb18)] = (this.rawout[1] + prvblk[ch][(sb18 + 1)]);
/* 1476 */       prvblk[ch][(sb18 + 1)] = this.rawout[19];
/* 1477 */       tsOut[(2 + sb18)] = (this.rawout[2] + prvblk[ch][(sb18 + 2)]);
/* 1478 */       prvblk[ch][(sb18 + 2)] = this.rawout[20];
/* 1479 */       tsOut[(3 + sb18)] = (this.rawout[3] + prvblk[ch][(sb18 + 3)]);
/* 1480 */       prvblk[ch][(sb18 + 3)] = this.rawout[21];
/* 1481 */       tsOut[(4 + sb18)] = (this.rawout[4] + prvblk[ch][(sb18 + 4)]);
/* 1482 */       prvblk[ch][(sb18 + 4)] = this.rawout[22];
/* 1483 */       tsOut[(5 + sb18)] = (this.rawout[5] + prvblk[ch][(sb18 + 5)]);
/* 1484 */       prvblk[ch][(sb18 + 5)] = this.rawout[23];
/* 1485 */       tsOut[(6 + sb18)] = (this.rawout[6] + prvblk[ch][(sb18 + 6)]);
/* 1486 */       prvblk[ch][(sb18 + 6)] = this.rawout[24];
/* 1487 */       tsOut[(7 + sb18)] = (this.rawout[7] + prvblk[ch][(sb18 + 7)]);
/* 1488 */       prvblk[ch][(sb18 + 7)] = this.rawout[25];
/* 1489 */       tsOut[(8 + sb18)] = (this.rawout[8] + prvblk[ch][(sb18 + 8)]);
/* 1490 */       prvblk[ch][(sb18 + 8)] = this.rawout[26];
/* 1491 */       tsOut[(9 + sb18)] = (this.rawout[9] + prvblk[ch][(sb18 + 9)]);
/* 1492 */       prvblk[ch][(sb18 + 9)] = this.rawout[27];
/* 1493 */       tsOut[(10 + sb18)] = (this.rawout[10] + prvblk[ch][(sb18 + 10)]);
/* 1494 */       prvblk[ch][(sb18 + 10)] = this.rawout[28];
/* 1495 */       tsOut[(11 + sb18)] = (this.rawout[11] + prvblk[ch][(sb18 + 11)]);
/* 1496 */       prvblk[ch][(sb18 + 11)] = this.rawout[29];
/* 1497 */       tsOut[(12 + sb18)] = (this.rawout[12] + prvblk[ch][(sb18 + 12)]);
/* 1498 */       prvblk[ch][(sb18 + 12)] = this.rawout[30];
/* 1499 */       tsOut[(13 + sb18)] = (this.rawout[13] + prvblk[ch][(sb18 + 13)]);
/* 1500 */       prvblk[ch][(sb18 + 13)] = this.rawout[31];
/* 1501 */       tsOut[(14 + sb18)] = (this.rawout[14] + prvblk[ch][(sb18 + 14)]);
/* 1502 */       prvblk[ch][(sb18 + 14)] = this.rawout[32];
/* 1503 */       tsOut[(15 + sb18)] = (this.rawout[15] + prvblk[ch][(sb18 + 15)]);
/* 1504 */       prvblk[ch][(sb18 + 15)] = this.rawout[33];
/* 1505 */       tsOut[(16 + sb18)] = (this.rawout[16] + prvblk[ch][(sb18 + 16)]);
/* 1506 */       prvblk[ch][(sb18 + 16)] = this.rawout[34];
/* 1507 */       tsOut[(17 + sb18)] = (this.rawout[17] + prvblk[ch][(sb18 + 17)]);
/* 1508 */       prvblk[ch][(sb18 + 17)] = this.rawout[35];
/*      */     }
/*      */   }
/*      */ 
/*      */   private void do_downmix()
/*      */   {
/* 1517 */     for (int sb = 0; sb < 18; sb++)
/* 1518 */       for (int ss = 0; ss < 18; ss += 3) {
/* 1519 */         this.lr[0][sb][ss] = ((this.lr[0][sb][ss] + this.lr[1][sb][ss]) * 0.5F);
/* 1520 */         this.lr[0][sb][(ss + 1)] = ((this.lr[0][sb][(ss + 1)] + this.lr[1][sb][(ss + 1)]) * 0.5F);
/* 1521 */         this.lr[0][sb][(ss + 2)] = ((this.lr[0][sb][(ss + 2)] + this.lr[1][sb][(ss + 2)]) * 0.5F);
/*      */       }
/*      */   }
/*      */ 
/*      */   public void inv_mdct(float[] in, float[] out, int block_type)
/*      */   {
/*      */     float tmpf_17;
/*      */     float tmpf_16;
/*      */     float tmpf_15;
/*      */     float tmpf_14;
/*      */     float tmpf_13;
/*      */     float tmpf_12;
/*      */     float tmpf_11;
/*      */     float tmpf_10;
/*      */     float tmpf_9;
/*      */     float tmpf_8;
/*      */     float tmpf_7;
/*      */     float tmpf_6;
/*      */     float tmpf_5;
/*      */     float tmpf_4;
/*      */     float tmpf_3;
/*      */     float tmpf_2;
/*      */     float tmpf_1;
/* 1538 */     float tmpf_0 = tmpf_1 = tmpf_2 = tmpf_3 = tmpf_4 = tmpf_5 = tmpf_6 = tmpf_7 = tmpf_8 = tmpf_9 = 
/* 1539 */       tmpf_10 = tmpf_11 = tmpf_12 = tmpf_13 = tmpf_14 = tmpf_15 = tmpf_16 = tmpf_17 = 0.0F;
/*      */ 
/* 1543 */     if (block_type == 2)
/*      */     {
/* 1557 */       out[0] = 0.0F;
/* 1558 */       out[1] = 0.0F;
/* 1559 */       out[2] = 0.0F;
/* 1560 */       out[3] = 0.0F;
/* 1561 */       out[4] = 0.0F;
/* 1562 */       out[5] = 0.0F;
/* 1563 */       out[6] = 0.0F;
/* 1564 */       out[7] = 0.0F;
/* 1565 */       out[8] = 0.0F;
/* 1566 */       out[9] = 0.0F;
/* 1567 */       out[10] = 0.0F;
/* 1568 */       out[11] = 0.0F;
/* 1569 */       out[12] = 0.0F;
/* 1570 */       out[13] = 0.0F;
/* 1571 */       out[14] = 0.0F;
/* 1572 */       out[15] = 0.0F;
/* 1573 */       out[16] = 0.0F;
/* 1574 */       out[17] = 0.0F;
/* 1575 */       out[18] = 0.0F;
/* 1576 */       out[19] = 0.0F;
/* 1577 */       out[20] = 0.0F;
/* 1578 */       out[21] = 0.0F;
/* 1579 */       out[22] = 0.0F;
/* 1580 */       out[23] = 0.0F;
/* 1581 */       out[24] = 0.0F;
/* 1582 */       out[25] = 0.0F;
/* 1583 */       out[26] = 0.0F;
/* 1584 */       out[27] = 0.0F;
/* 1585 */       out[28] = 0.0F;
/* 1586 */       out[29] = 0.0F;
/* 1587 */       out[30] = 0.0F;
/* 1588 */       out[31] = 0.0F;
/* 1589 */       out[32] = 0.0F;
/* 1590 */       out[33] = 0.0F;
/* 1591 */       out[34] = 0.0F;
/* 1592 */       out[35] = 0.0F;
/*      */ 
/* 1594 */       int six_i = 0;
/*      */ 
/* 1596 */       for (int i = 0; i < 3; i++)
/*      */       {
/* 1601 */         in[(15 + i)] += in[(12 + i)]; in[(12 + i)] += in[(9 + i)]; in[(9 + i)] += in[(6 + i)];
/* 1602 */         in[(6 + i)] += in[(3 + i)]; in[(3 + i)] += in[(0 + i)];
/*      */ 
/* 1605 */         in[(15 + i)] += in[(9 + i)]; in[(9 + i)] += in[(3 + i)];
/*      */ 
/* 1609 */         float pp2 = in[(12 + i)] * 0.5F;
/* 1610 */         float pp1 = in[(6 + i)] * 0.8660254F;
/* 1611 */         float sum = in[(0 + i)] + pp2;
/* 1612 */         tmpf_1 = in[(0 + i)] - in[(12 + i)];
/* 1613 */         tmpf_0 = sum + pp1;
/* 1614 */         tmpf_2 = sum - pp1;
/*      */ 
/* 1618 */         pp2 = in[(15 + i)] * 0.5F;
/* 1619 */         pp1 = in[(9 + i)] * 0.8660254F;
/* 1620 */         sum = in[(3 + i)] + pp2;
/* 1621 */         tmpf_4 = in[(3 + i)] - in[(15 + i)];
/* 1622 */         tmpf_5 = sum + pp1;
/* 1623 */         tmpf_3 = sum - pp1;
/*      */ 
/* 1627 */         tmpf_3 *= 1.931852F;
/* 1628 */         tmpf_4 *= 0.7071068F;
/* 1629 */         tmpf_5 *= 0.5176381F;
/*      */ 
/* 1632 */         float save = tmpf_0;
/* 1633 */         tmpf_0 += tmpf_5;
/* 1634 */         tmpf_5 = save - tmpf_5;
/* 1635 */         save = tmpf_1;
/* 1636 */         tmpf_1 += tmpf_4;
/* 1637 */         tmpf_4 = save - tmpf_4;
/* 1638 */         save = tmpf_2;
/* 1639 */         tmpf_2 += tmpf_3;
/* 1640 */         tmpf_3 = save - tmpf_3;
/*      */ 
/* 1645 */         tmpf_0 *= 0.5043145F;
/* 1646 */         tmpf_1 *= 0.5411961F;
/* 1647 */         tmpf_2 *= 0.6302362F;
/* 1648 */         tmpf_3 *= 0.8213398F;
/* 1649 */         tmpf_4 *= 1.306563F;
/* 1650 */         tmpf_5 *= 3.830649F;
/*      */ 
/* 1655 */         tmpf_8 = -tmpf_0 * 0.7933533F;
/* 1656 */         tmpf_9 = -tmpf_0 * 0.6087614F;
/* 1657 */         tmpf_7 = -tmpf_1 * 0.92388F;
/* 1658 */         tmpf_10 = -tmpf_1 * 0.3826834F;
/* 1659 */         tmpf_6 = -tmpf_2 * 0.9914449F;
/* 1660 */         tmpf_11 = -tmpf_2 * 0.1305262F;
/*      */ 
/* 1662 */         tmpf_0 = tmpf_3;
/* 1663 */         tmpf_1 = tmpf_4 * 0.3826834F;
/* 1664 */         tmpf_2 = tmpf_5 * 0.6087614F;
/*      */ 
/* 1666 */         tmpf_3 = -tmpf_5 * 0.7933533F;
/* 1667 */         tmpf_4 = -tmpf_4 * 0.92388F;
/* 1668 */         tmpf_5 = -tmpf_0 * 0.9914449F;
/*      */ 
/* 1670 */         tmpf_0 *= 0.1305262F;
/*      */ 
/* 1672 */         out[(six_i + 6)] += tmpf_0;
/* 1673 */         out[(six_i + 7)] += tmpf_1;
/* 1674 */         out[(six_i + 8)] += tmpf_2;
/* 1675 */         out[(six_i + 9)] += tmpf_3;
/* 1676 */         out[(six_i + 10)] += tmpf_4;
/* 1677 */         out[(six_i + 11)] += tmpf_5;
/* 1678 */         out[(six_i + 12)] += tmpf_6;
/* 1679 */         out[(six_i + 13)] += tmpf_7;
/* 1680 */         out[(six_i + 14)] += tmpf_8;
/* 1681 */         out[(six_i + 15)] += tmpf_9;
/* 1682 */         out[(six_i + 16)] += tmpf_10;
/* 1683 */         out[(six_i + 17)] += tmpf_11;
/*      */ 
/* 1685 */         six_i += 6;
/*      */       }
/*      */ 
/*      */     }
/*      */     else
/*      */     {
/* 1692 */       in[17] += in[16]; in[16] += in[15]; in[15] += in[14]; in[14] += in[13];
/* 1693 */       in[13] += in[12]; in[12] += in[11]; in[11] += in[10]; in[10] += in[9];
/* 1694 */       in[9] += in[8]; in[8] += in[7]; in[7] += in[6]; in[6] += in[5];
/* 1695 */       in[5] += in[4]; in[4] += in[3]; in[3] += in[2]; in[2] += in[1];
/* 1696 */       in[1] += in[0];
/*      */ 
/* 1700 */       in[17] += in[15]; in[15] += in[13]; in[13] += in[11]; in[11] += in[9];
/* 1701 */       in[9] += in[7]; in[7] += in[5]; in[5] += in[3]; in[3] += in[1];
/*      */ 
/* 1720 */       float i00 = in[0] + in[0];
/* 1721 */       float iip12 = i00 + in[12];
/*      */ 
/* 1723 */       float tmp0 = iip12 + in[4] * 1.879385F + in[8] * 1.532089F + in[16] * 0.3472964F;
/* 1724 */       float tmp1 = i00 + in[4] - in[8] - in[12] - in[12] - in[16];
/* 1725 */       float tmp2 = iip12 - in[4] * 0.3472964F - in[8] * 1.879385F + in[16] * 1.532089F;
/* 1726 */       float tmp3 = iip12 - in[4] * 1.532089F + in[8] * 0.3472964F - in[16] * 1.879385F;
/* 1727 */       float tmp4 = in[0] - in[4] + in[8] - in[12] + in[16];
/*      */ 
/* 1730 */       float i66_ = in[6] * 1.732051F;
/*      */ 
/* 1732 */       float tmp0_ = in[2] * 1.969616F + i66_ + in[10] * 1.285575F + in[14] * 0.6840403F;
/* 1733 */       float tmp1_ = (in[2] - in[10] - in[14]) * 1.732051F;
/* 1734 */       float tmp2_ = in[2] * 1.285575F - i66_ - in[10] * 0.6840403F + in[14] * 1.969616F;
/* 1735 */       float tmp3_ = in[2] * 0.6840403F - i66_ + in[10] * 1.969616F - in[14] * 1.285575F;
/*      */ 
/* 1739 */       float i0 = in[1] + in[1];
/* 1740 */       float i0p12 = i0 + in[13];
/*      */ 
/* 1742 */       float tmp0o = i0p12 + in[5] * 1.879385F + in[9] * 1.532089F + in[17] * 0.3472964F;
/* 1743 */       float tmp1o = i0 + in[5] - in[9] - in[13] - in[13] - in[17];
/* 1744 */       float tmp2o = i0p12 - in[5] * 0.3472964F - in[9] * 1.879385F + in[17] * 1.532089F;
/* 1745 */       float tmp3o = i0p12 - in[5] * 1.532089F + in[9] * 0.3472964F - in[17] * 1.879385F;
/* 1746 */       float tmp4o = (in[1] - in[5] + in[9] - in[13] + in[17]) * 0.7071068F;
/*      */ 
/* 1749 */       float i6_ = in[7] * 1.732051F;
/*      */ 
/* 1751 */       float tmp0_o = in[3] * 1.969616F + i6_ + in[11] * 1.285575F + in[15] * 0.6840403F;
/* 1752 */       float tmp1_o = (in[3] - in[11] - in[15]) * 1.732051F;
/* 1753 */       float tmp2_o = in[3] * 1.285575F - i6_ - in[11] * 0.6840403F + in[15] * 1.969616F;
/* 1754 */       float tmp3_o = in[3] * 0.6840403F - i6_ + in[11] * 1.969616F - in[15] * 1.285575F;
/*      */ 
/* 1763 */       float e = tmp0 + tmp0_; float o = (tmp0o + tmp0_o) * 0.5019099F; tmpf_0 = e + o; tmpf_17 = e - o;
/* 1764 */       e = tmp1 + tmp1_; o = (tmp1o + tmp1_o) * 0.5176381F; tmpf_1 = e + o; tmpf_16 = e - o;
/* 1765 */       e = tmp2 + tmp2_; o = (tmp2o + tmp2_o) * 0.551689F; tmpf_2 = e + o; tmpf_15 = e - o;
/* 1766 */       e = tmp3 + tmp3_; o = (tmp3o + tmp3_o) * 0.6103873F; tmpf_3 = e + o; tmpf_14 = e - o;
/* 1767 */       tmpf_4 = tmp4 + tmp4o; tmpf_13 = tmp4 - tmp4o;
/* 1768 */       e = tmp3 - tmp3_; o = (tmp3o - tmp3_o) * 0.8717234F; tmpf_5 = e + o; tmpf_12 = e - o;
/* 1769 */       e = tmp2 - tmp2_; o = (tmp2o - tmp2_o) * 1.183101F; tmpf_6 = e + o; tmpf_11 = e - o;
/* 1770 */       e = tmp1 - tmp1_; o = (tmp1o - tmp1_o) * 1.931852F; tmpf_7 = e + o; tmpf_10 = e - o;
/* 1771 */       e = tmp0 - tmp0_; o = (tmp0o - tmp0_o) * 5.736857F; tmpf_8 = e + o; tmpf_9 = e - o;
/*      */ 
/* 1775 */       float[] win_bt = win[block_type];
/*      */ 
/* 1777 */       out[0] = (-tmpf_9 * win_bt[0]);
/* 1778 */       out[1] = (-tmpf_10 * win_bt[1]);
/* 1779 */       out[2] = (-tmpf_11 * win_bt[2]);
/* 1780 */       out[3] = (-tmpf_12 * win_bt[3]);
/* 1781 */       out[4] = (-tmpf_13 * win_bt[4]);
/* 1782 */       out[5] = (-tmpf_14 * win_bt[5]);
/* 1783 */       out[6] = (-tmpf_15 * win_bt[6]);
/* 1784 */       out[7] = (-tmpf_16 * win_bt[7]);
/* 1785 */       out[8] = (-tmpf_17 * win_bt[8]);
/* 1786 */       out[9] = (tmpf_17 * win_bt[9]);
/* 1787 */       out[10] = (tmpf_16 * win_bt[10]);
/* 1788 */       out[11] = (tmpf_15 * win_bt[11]);
/* 1789 */       out[12] = (tmpf_14 * win_bt[12]);
/* 1790 */       out[13] = (tmpf_13 * win_bt[13]);
/* 1791 */       out[14] = (tmpf_12 * win_bt[14]);
/* 1792 */       out[15] = (tmpf_11 * win_bt[15]);
/* 1793 */       out[16] = (tmpf_10 * win_bt[16]);
/* 1794 */       out[17] = (tmpf_9 * win_bt[17]);
/* 1795 */       out[18] = (tmpf_8 * win_bt[18]);
/* 1796 */       out[19] = (tmpf_7 * win_bt[19]);
/* 1797 */       out[20] = (tmpf_6 * win_bt[20]);
/* 1798 */       out[21] = (tmpf_5 * win_bt[21]);
/* 1799 */       out[22] = (tmpf_4 * win_bt[22]);
/* 1800 */       out[23] = (tmpf_3 * win_bt[23]);
/* 1801 */       out[24] = (tmpf_2 * win_bt[24]);
/* 1802 */       out[25] = (tmpf_1 * win_bt[25]);
/* 1803 */       out[26] = (tmpf_0 * win_bt[26]);
/* 1804 */       out[27] = (tmpf_0 * win_bt[27]);
/* 1805 */       out[28] = (tmpf_1 * win_bt[28]);
/* 1806 */       out[29] = (tmpf_2 * win_bt[29]);
/* 1807 */       out[30] = (tmpf_3 * win_bt[30]);
/* 1808 */       out[31] = (tmpf_4 * win_bt[31]);
/* 1809 */       out[32] = (tmpf_5 * win_bt[32]);
/* 1810 */       out[33] = (tmpf_6 * win_bt[33]);
/* 1811 */       out[34] = (tmpf_7 * win_bt[34]);
/* 1812 */       out[35] = (tmpf_8 * win_bt[35]);
/*      */     }
/*      */   }
/*      */ 
/*      */   private static float[] create_t_43()
/*      */   {
/* 1966 */     float[] t43 = new float[8192];
/* 1967 */     double d43 = 1.333333333333333D;
/*      */ 
/* 1969 */     for (int i = 0; i < 8192; i++)
/*      */     {
/* 1971 */       t43[i] = (float)Math.pow(i, 1.333333333333333D);
/*      */     }
/* 1973 */     return t43;
/*      */   }
/*      */ 
/*      */   static int[] reorder(int[] scalefac_band)
/*      */   {
/* 2103 */     int j = 0;
/* 2104 */     int[] ix = new int[576];
/* 2105 */     for (int sfb = 0; sfb < 13; sfb++) {
/* 2106 */       int start = scalefac_band[sfb];
/* 2107 */       int end = scalefac_band[(sfb + 1)];
/* 2108 */       for (int window = 0; window < 3; window++)
/* 2109 */         for (int i = start; i < end; i++)
/* 2110 */           ix[(3 * i + window)] = (j++);
/*      */     }
/* 2112 */     return ix;
/*      */   }
/*      */ 
/*      */   static class SBI
/*      */   {
/*      */     public int[] l;
/*      */     public int[] s;
/*      */ 
/*      */     public SBI()
/*      */     {
/* 1834 */       this.l = new int[23];
/* 1835 */       this.s = new int[14];
/*      */     }
/*      */ 
/*      */     public SBI(int[] thel, int[] thes) {
/* 1839 */       this.l = thel;
/* 1840 */       this.s = thes;
/*      */     }
/*      */   }
/*      */ 
/*      */   static class gr_info_s
/*      */   {
/* 1846 */     public int part2_3_length = 0;
/* 1847 */     public int big_values = 0;
/* 1848 */     public int global_gain = 0;
/* 1849 */     public int scalefac_compress = 0;
/* 1850 */     public int window_switching_flag = 0;
/* 1851 */     public int block_type = 0;
/* 1852 */     public int mixed_block_flag = 0;
/*      */     public int[] table_select;
/*      */     public int[] subblock_gain;
/* 1855 */     public int region0_count = 0;
/* 1856 */     public int region1_count = 0;
/* 1857 */     public int preflag = 0;
/* 1858 */     public int scalefac_scale = 0;
/* 1859 */     public int count1table_select = 0;
/*      */ 
/*      */     public gr_info_s()
/*      */     {
/* 1866 */       this.table_select = new int[3];
/* 1867 */       this.subblock_gain = new int[3];
/*      */     }
/*      */   }
/*      */ 
/*      */   static class temporaire
/*      */   {
/*      */     public int[] scfsi;
/*      */     public LayerIIIDecoder.gr_info_s[] gr;
/*      */ 
/*      */     public temporaire()
/*      */     {
/* 1881 */       this.scfsi = new int[4];
/* 1882 */       this.gr = new LayerIIIDecoder.gr_info_s[2];
/* 1883 */       this.gr[0] = new LayerIIIDecoder.gr_info_s();
/* 1884 */       this.gr[1] = new LayerIIIDecoder.gr_info_s();
/*      */     }
/*      */   }
/*      */ 
/*      */   static class III_side_info_t
/*      */   {
/* 1891 */     public int main_data_begin = 0;
/* 1892 */     public int private_bits = 0;
/*      */     public LayerIIIDecoder.temporaire[] ch;
/*      */ 
/*      */     public III_side_info_t() {
/* 1899 */       this.ch = new LayerIIIDecoder.temporaire[2];
/* 1900 */       this.ch[0] = new LayerIIIDecoder.temporaire();
/* 1901 */       this.ch[1] = new LayerIIIDecoder.temporaire();
/*      */     }
/*      */   }
/*      */ 
/*      */   static class temporaire2
/*      */   {
/*      */     public int[] l;
/*      */     public int[][] s;
/*      */ 
/*      */     public temporaire2()
/*      */     {
/* 1915 */       this.l = new int[23];
/* 1916 */       this.s = new int[3][13];
/*      */     }
/*      */   }
/*      */ 
/*      */   class Sftable
/*      */   {
/*      */     public int[] l;
/*      */     public int[] s;
/*      */ 
/*      */     public Sftable()
/*      */     {
/* 2417 */       this.l = new int[5];
/* 2418 */       this.s = new int[3];
/*      */     }
/*      */ 
/*      */     public Sftable(int[] thel, int[] thes)
/*      */     {
/* 2423 */       this.l = thel;
/* 2424 */       this.s = thes;
/*      */     }
/*      */   }
/*      */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     javazoom.jl.decoder.LayerIIIDecoder
 * JD-Core Version:    0.6.0
 */