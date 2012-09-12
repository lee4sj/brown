/*      */ package kj.player;
/*      */ 
/*      */ import java.io.PrintStream;
/*      */ import java.util.ArrayList;
/*      */ import javax.sound.sampled.AudioFormat;
/*      */ import javax.sound.sampled.AudioSystem;
/*      */ import javax.sound.sampled.DataLine.Info;
/*      */ import javax.sound.sampled.FloatControl;
/*      */ import javax.sound.sampled.FloatControl.Type;
/*      */ import javax.sound.sampled.LineUnavailableException;
/*      */ import javax.sound.sampled.Mixer;
/*      */ import javax.sound.sampled.Mixer.Info;
/*      */ import javax.sound.sampled.SourceDataLine;
/*      */ import kj.audio.KJAudioDataConsumer;
/*      */ import kj.audio.KJAudioDataProvider;
/*      */ import kj.dsp.KJDigitalSignalProcessingAudioDataConsumer;
/*      */ import kj.dsp.KJDigitalSignalProcessor;
/*      */ import kj.track.KJAbstractTrackProvider;
/*      */ import kj.track.KJTrack;
/*      */ import kj.track.KJTrackCrossFade;
/*      */ import kj.track.KJTrackProvider;
/*      */ import kj.ui.KJDSPDialog;
/*      */ import kj.ui.KJJukeBox;
/*      */ 
/*      */ public class KJPlayer
/*      */ {
/*      */   private static final int AUDIO_BUFFER_SIZE = 131072;
/*      */   private static final int HISTORY_SIZE = 100;
/*      */   private static final int STATUS_STOPPED = 1;
/*      */   private static final int STATUS_PAUSED = 2;
/*      */   private static final int STATUS_PLAYING = 3;
/*   44 */   private static final double LOG_10 = Math.log(10.0D);
/*      */ 
/*   46 */   private int status = 1;
/*      */   private KJTrackProvider trackProvider;
/*      */   private TrackThread trackThread;
/*   51 */   private Object trackThreadLock = new Object();
/*      */   private PlayThread currentPlayThread;
/*   54 */   private Object playThreadLock = new Object();
/*      */ 
/*   56 */   private Object pauseLock = new Object();
/*   57 */   private float masterGain = 100.0F;
/*      */ 
/*   59 */   private ArrayList playerListeners = new ArrayList();
/*      */ 
/*   61 */   private ArrayList historyList = new ArrayList();
/*   62 */   private int historyIndex = 0;
/*   63 */   private boolean historyIndexAdjusted = false;
/*      */   private KJTrack nextUserTrack;
/*   67 */   private boolean markPlayedTracks = true;
/*      */ 
/*   69 */   private boolean fadeWhenNavigating = false;
/*   70 */   private KJTrackCrossFade navigationCrossFade = null;
/*      */ 
/*   72 */   private KJDigitalSignalProcessingAudioDataConsumer dsp = new KJDigitalSignalProcessingAudioDataConsumer();
/*      */ 
/*   74 */   private VolumeMonitorDSP vmd = new VolumeMonitorDSP(null);
/*      */ 
/*   76 */   private boolean monitorVolume = true;
/*      */   private long monitoringDelay;
/*      */ 
/*      */   public KJPlayer()
/*      */   {
/*   80 */     this(null);
/*      */   }
/*      */ 
/*      */   public KJPlayer(KJTrack pTrack) {
/*   84 */     this(new SingleTrackProvider(pTrack));
/*      */   }
/*      */ 
/*      */   public KJPlayer(KJTrackProvider pTrackProvider) {
/*   88 */     setTrackProvider(pTrackProvider);
/*      */   }
/*      */ 
/*      */   public void addDigitalSignalProcessor(KJDigitalSignalProcessor pDsp) {
/*   92 */     this.dsp.add(pDsp);
/*      */   }
/*      */ 
/*      */   public void addListener(KJPlayerListener pListener) {
/*   96 */     this.playerListeners.add(pListener);
/*      */   }
/*      */ 
/*      */   protected void addToTrackHistory(KJTrack pTrack)
/*      */   {
/*  101 */     this.historyList.add(pTrack);
/*      */ 
/*  103 */     if (this.historyList.size() > 100) {
/*  104 */       this.historyList.remove(0);
/*      */     }
/*      */ 
/*  107 */     this.historyIndex = this.historyList.size();
/*      */   }
/*      */ 
/*      */   protected synchronized SourceDataLine createSourceDataLine(KJAudioDataProvider pAdp)
/*      */   {
/*      */     try
/*      */     {
/*  116 */       AudioFormat wTrackFormat = new AudioFormat(pAdp.frequency(), 
/*  117 */         pAdp.sampleSize(), pAdp.channels(), true, false);
/*      */ 
/*  119 */       DataLine.Info wLi = new DataLine.Info(SourceDataLine.class, wTrackFormat);
/*      */ 
/*  121 */       Mixer wMixer = getDefaultMixer();
/*      */ 
/*  123 */       SourceDataLine wSdl = (SourceDataLine)wMixer.getLine(wLi);
/*      */ 
/*  125 */       wSdl.open(wTrackFormat, 131072);
/*      */ 
/*  127 */       return wSdl;
/*      */     } catch (Exception pEx) {
/*      */     }
/*  130 */     throw new KJPlayerException(pEx);
/*      */   }
/*      */ 
/*      */   protected void endCurrentTrack()
/*      */   {
/*  137 */     synchronized (this.playThreadLock)
/*      */     {
/*  139 */       if ((this.currentPlayThread != null) && (!this.currentPlayThread.isStopped()))
/*      */       {
/*  141 */         if ((this.fadeWhenNavigating) && (this.navigationCrossFade != null))
/*  142 */           this.currentPlayThread.getFadeMonitor().fadeNow(this.navigationCrossFade);
/*      */         else
/*  144 */           this.currentPlayThread.stop();
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   public void fadeGain(float pTo, long pDurationInMilliseconds)
/*      */   {
/*  154 */     fadeGain(getGain(), pTo, pDurationInMilliseconds);
/*      */   }
/*      */ 
/*      */   public void fadeGain(float pFrom, float pTo, long pDurationInMilliseconds)
/*      */   {
/*  159 */     float wFadeRange = pTo - pFrom;
/*      */ 
/*  161 */     long wCfs = System.currentTimeMillis();
/*  162 */     long wFadePosition = 0L;
/*      */ 
/*  164 */     while (wFadePosition < pDurationInMilliseconds)
/*      */     {
/*  166 */       setGain(pFrom + 
/*  167 */         wFadeRange * ((float)wFadePosition / (float)pDurationInMilliseconds));
/*      */ 
/*  169 */       wFadePosition = System.currentTimeMillis() - wCfs;
/*      */       try
/*      */       {
/*  172 */         Thread.sleep(100L);
/*      */       } catch (InterruptedException pEx) {
/*  174 */         pEx.printStackTrace();
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   protected void firePlayerEvent(int pType, KJTrack pTrack)
/*      */   {
/*  183 */     KJPlayerEvent wEvent = new KJPlayerEvent(this, pType, pTrack);
/*      */ 
/*  185 */     for (int a = 0; a < this.playerListeners.size(); a++)
/*  186 */       ((KJPlayerListener)this.playerListeners.get(a)).playerEvent(wEvent);
/*      */   }
/*      */ 
/*      */   protected FloatControl getMasterGainControl(SourceDataLine pSdl)
/*      */   {
/*  192 */     return (FloatControl)pSdl.getControl(FloatControl.Type.MASTER_GAIN);
/*      */   }
/*      */ 
/*      */   public KJTrackCrossFade getNavigationCrossFader() {
/*  196 */     return this.navigationCrossFade;
/*      */   }
/*      */ 
/*      */   public KJTrack getNextTrack()
/*      */   {
/*  201 */     if (this.historyIndex < this.historyList.size() - 1) {
/*  202 */       return (KJTrack)this.historyList.get(this.historyIndex + 1);
/*      */     }
/*      */ 
/*  205 */     return this.nextUserTrack;
/*      */   }
/*      */ 
/*      */   public KJAudioDataProvider getCurrentAudioDataProvider()
/*      */   {
/*  211 */     synchronized (this.playThreadLock)
/*      */     {
/*  213 */       PlayThread wCpt = getCurrentPlayThread();
/*      */ 
/*  215 */       if (wCpt != null) {
/*  216 */         return wCpt.getAudioDataProvider();
/*      */       }
/*  218 */       return null;
/*      */     }
/*      */   }
/*      */ 
/*      */   public long getCurrentPosition()
/*      */   {
/*  227 */     synchronized (this.playThreadLock)
/*      */     {
/*  229 */       PlayThread wCpt = getCurrentPlayThread();
/*      */ 
/*  231 */       if (wCpt != null) {
/*  232 */         return wCpt.getPositionInMilliseconds();
/*      */       }
/*  234 */       return 0L;
/*      */     }
/*      */   }
/*      */ 
/*      */   private PlayThread getCurrentPlayThread()
/*      */   {
/*  242 */     return this.currentPlayThread;
/*      */   }
/*      */ 
/*      */   public KJTrack getCurrentTrack()
/*      */   {
/*  247 */     synchronized (this.playThreadLock)
/*      */     {
/*  249 */       if (this.currentPlayThread != null) {
/*  250 */         return this.currentPlayThread.getTrack();
/*      */       }
/*  252 */       return null;
/*      */     }
/*      */   }
/*      */ 
/*      */   public KJTrackProvider getTrackProvider()
/*      */   {
/*  260 */     return this.trackProvider;
/*      */   }
/*      */ 
/*      */   protected Mixer getDefaultMixer()
/*      */   {
/*  265 */     Mixer.Info[] wMixerInfo = AudioSystem.getMixerInfo();
/*      */ 
/*  267 */     return AudioSystem.getMixer(wMixerInfo[0]);
/*      */   }
/*      */ 
/*      */   public float getGain()
/*      */   {
/*  272 */     return this.masterGain;
/*      */   }
/*      */ 
/*      */   public float getMaximumGainPercentage()
/*      */   {
/*      */     try
/*      */     {
/*  279 */       Mixer wMixer = getDefaultMixer();
/*      */ 
/*  281 */       SourceDataLine wSdl = (SourceDataLine)wMixer.getLine(
/*  282 */         new DataLine.Info(SourceDataLine.class, null));
/*      */ 
/*  284 */       wSdl.open();
/*      */ 
/*  286 */       FloatControl wMgc = getMasterGainControl(wSdl);
/*      */ 
/*  288 */       wSdl.close();
/*      */ 
/*  290 */       return (float)Math.pow(10.0D, 
/*  291 */         (1.0D - wMgc.getMaximum() / wMgc.getMinimum()) * 2.0D);
/*      */     }
/*      */     catch (LineUnavailableException pEx)
/*      */     {
/*  295 */       pEx.printStackTrace();
/*      */     }
/*      */ 
/*  298 */     return 0.0F;
/*      */   }
/*      */ 
/*      */   public boolean isMarkingPlayedTracks()
/*      */   {
/*  303 */     return this.markPlayedTracks;
/*      */   }
/*      */ 
/*      */   public boolean isPlaying() {
/*  307 */     return this.status == 3;
/*      */   }
/*      */ 
/*      */   public boolean isStopped() {
/*  311 */     return this.status == 1;
/*      */   }
/*      */ 
/*      */   public boolean isVolumeMonitoringEnabled() {
/*  315 */     return this.monitorVolume;
/*      */   }
/*      */ 
/*      */   public void next()
/*      */   {
/*  320 */     if (this.historyIndex < this.historyList.size()) {
/*  321 */       this.historyIndex += 1;
/*  322 */       this.historyIndexAdjusted = true;
/*      */     }
/*      */ 
/*  325 */     endCurrentTrack();
/*      */   }
/*      */ 
/*      */   public void next(KJTrack pNextTrack)
/*      */   {
/*  335 */     this.nextUserTrack = pNextTrack;
/*      */ 
/*  337 */     firePlayerEvent(6, pNextTrack);
/*      */   }
/*      */ 
/*      */   public void pause()
/*      */   {
/*  343 */     if (this.status == 2) {
/*  344 */       unpause();
/*  345 */     } else if (this.status == 3) {
/*  346 */       this.status = 2;
/*  347 */       firePlayerEvent(2, getCurrentTrack());
/*      */     }
/*      */   }
/*      */ 
/*      */   private void unpause()
/*      */   {
/*  354 */     if (this.status != 1)
/*      */     {
/*  356 */       this.status = 3;
/*      */ 
/*  358 */       synchronized (this.pauseLock) {
/*  359 */         this.pauseLock.notifyAll();
/*      */       }
/*      */ 
/*  362 */       firePlayerEvent(1, getCurrentTrack());
/*      */     }
/*      */   }
/*      */ 
/*      */   protected float percentToMasterGainControlValue(float pPercent, FloatControl pMasterGainControl)
/*      */   {
/*  372 */     if (pPercent >= 1.0F)
/*      */     {
/*  374 */       float wDb = (1.0F - (float)(Math.log(pPercent) / LOG_10) / 2.0F) * 
/*  375 */         pMasterGainControl.getMinimum();
/*      */ 
/*  378 */       if (wDb > pMasterGainControl.getMaximum()) {
/*  379 */         wDb = pMasterGainControl.getMaximum();
/*      */       }
/*      */ 
/*  382 */       return wDb;
/*      */     }
/*      */ 
/*  385 */     return pMasterGainControl.getMinimum();
/*      */   }
/*      */ 
/*      */   public void play()
/*      */   {
/*  392 */     synchronized (this.trackThreadLock)
/*      */     {
/*  394 */       if ((this.trackThread != null) && (this.trackThread.isAlive()))
/*      */       {
/*  397 */         if (this.status == 2) {
/*  398 */           unpause();
/*      */         }
/*      */ 
/*  401 */         return;
/*      */       }
/*      */ 
/*  405 */       this.status = 3;
/*      */ 
/*  407 */       this.trackThread = new TrackThread(null);
/*  408 */       this.trackThread.start();
/*      */     }
/*      */   }
/*      */ 
/*      */   public void previous()
/*      */   {
/*  417 */     if (this.historyIndex == this.historyList.size()) {
/*  418 */       this.historyIndex -= 1;
/*      */     }
/*      */ 
/*  421 */     this.historyIndex -= 1;
/*      */ 
/*  423 */     if (this.historyIndex < 0) {
/*  424 */       this.historyIndex = 0;
/*      */     }
/*      */ 
/*  427 */     this.historyIndexAdjusted = true;
/*      */ 
/*  429 */     endCurrentTrack();
/*      */   }
/*      */ 
/*      */   public void removeDigitalSignalProcessor(KJDigitalSignalProcessor pDsp)
/*      */   {
/*  434 */     this.dsp.remove(pDsp);
/*      */   }
/*      */ 
/*      */   public void removeListener(KJPlayerListener pListener) {
/*  438 */     this.playerListeners.remove(pListener);
/*      */   }
/*      */ 
/*      */   public void seek(long pPosition)
/*      */   {
/*  443 */     synchronized (this.playThreadLock)
/*      */     {
/*  445 */       PlayThread wCpt = getCurrentPlayThread();
/*      */ 
/*  447 */       if (wCpt != null)
/*  448 */         wCpt.seek(pPosition);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void setCurrentPlayThread(PlayThread pPlayThread)
/*      */   {
/*  456 */     this.currentPlayThread = pPlayThread;
/*      */   }
/*      */ 
/*      */   public void setFadeWhenNavigating(boolean pState) {
/*  460 */     this.fadeWhenNavigating = pState;
/*      */   }
/*      */ 
/*      */   private void setSourceDataLineGainPercentage(float pPercent, SourceDataLine pSdl)
/*      */   {
/*  466 */     FloatControl wMgc = getMasterGainControl(pSdl);
/*      */ 
/*  468 */     float wDb = percentToMasterGainControlValue(pPercent, wMgc);
/*      */ 
/*  472 */     wMgc.setValue(wDb);
/*      */   }
/*      */ 
/*      */   public void setTrackProvider(KJTrackProvider pTrackProvider)
/*      */   {
/*  478 */     synchronized (this.playThreadLock) {
/*  479 */       this.trackProvider = pTrackProvider;
/*      */     }
/*      */   }
/*      */ 
/*      */   public void setGain(float pPercent)
/*      */   {
/*  485 */     this.masterGain = pPercent;
/*      */   }
/*      */ 
/*      */   public void setMarkPlayedTracks(boolean pState) {
/*  489 */     this.markPlayedTracks = pState;
/*      */   }
/*      */ 
/*      */   public void setNavigationCrossFader(KJTrackCrossFade pCrossFader) {
/*  493 */     this.navigationCrossFade = pCrossFader;
/*      */   }
/*      */ 
/*      */   public void setVolumeMonitoringDelay(long pDelay) {
/*  497 */     this.monitoringDelay = pDelay;
/*      */   }
/*      */ 
/*      */   public void setVolumeMonitoringEnabled(boolean pState)
/*      */   {
/*  502 */     removeListener(this.vmd);
/*  503 */     removeDigitalSignalProcessor(this.vmd);
/*      */ 
/*  505 */     if (pState)
/*      */     {
/*  507 */       addListener(this.vmd);
/*  508 */       addDigitalSignalProcessor(this.vmd);
/*      */     }
/*      */ 
/*  512 */     this.monitorVolume = pState;
/*      */   }
/*      */ 
/*      */   public void repeat()
/*      */   {
/*  517 */     next(getCurrentTrack());
/*      */   }
/*      */ 
/*      */   public void stop()
/*      */   {
/*  522 */     this.status = 1;
/*      */ 
/*  525 */     this.dsp.stop();
/*      */ 
/*  528 */     synchronized (this.pauseLock) {
/*  529 */       this.pauseLock.notifyAll();
/*      */     }
/*      */ 
/*  532 */     firePlayerEvent(3, getCurrentTrack());
/*      */   }
/*      */ 
/*      */   private class CompositeAudioDataConsumer implements KJAudioDataConsumer
/*      */   {
/*      */     private KJAudioDataConsumer adc1;
/*      */     private KJAudioDataConsumer adc2;
/*      */ 
/*      */     public CompositeAudioDataConsumer(KJAudioDataConsumer pAdc1, KJAudioDataConsumer pAdc2) {
/*  543 */       this.adc1 = pAdc1;
/*  544 */       this.adc2 = pAdc2;
/*      */     }
/*      */ 
/*      */     public void writeAudioData(byte[] pAudioData, boolean encrypted)
/*      */     {
/*  549 */       this.adc1.writeAudioData(pAudioData, encrypted);
/*  550 */       this.adc2.writeAudioData(pAudioData, encrypted);
/*      */     }
/*      */ 
/*      */     public void writeAudioData(byte[] pAudioData, int pOffset, int pLength, boolean encrypted)
/*      */     {
/*  560 */       this.adc1.writeAudioData(pAudioData, pOffset, pLength, encrypted);
/*  561 */       this.adc2.writeAudioData(pAudioData, pOffset, pLength, encrypted);
/*      */     }
/*      */   }
/*      */ 
/*      */   private class FadeMonitor extends Thread {
/*      */     private KJPlayer.PlayThread playThread;
/*  570 */     private boolean fading = false;
/*      */     private KJTrackCrossFade crossFade;
/*  573 */     private long crossFadeStartTime = -1L;
/*  574 */     private float faderGain = 100.0F;
/*      */     private float fadeRange;
/*      */ 
/*      */     public FadeMonitor(KJPlayer.PlayThread pPlayThread) {
/*  579 */       this.playThread = pPlayThread;
/*      */ 
/*  581 */       this.crossFade = this.playThread.getTrack().getResolvedCrossFader();
/*      */ 
/*  583 */       initialize();
/*      */     }
/*      */ 
/*      */     public void fadeNow(KJTrackCrossFade pCrossFade)
/*      */     {
/*  589 */       synchronized (this)
/*      */       {
/*  591 */         if (!this.fading)
/*      */         {
/*  593 */           this.crossFade = pCrossFade;
/*      */ 
/*  595 */           initialize();
/*      */ 
/*  597 */           startFading();
/*      */         }
/*      */       }
/*      */     }
/*      */ 
/*      */     public long getCrossFadePositionInMilliseconds()
/*      */     {
/*  607 */       if (this.crossFadeStartTime != -1L) {
/*  608 */         return System.currentTimeMillis() - this.crossFadeStartTime;
/*      */       }
/*  610 */       return -1L;
/*      */     }
/*      */ 
/*      */     public float getCompositeGain()
/*      */     {
/*  616 */       return KJPlayer.this.masterGain / 100.0F * this.faderGain * (
/*  617 */         this.playThread.getTrack().getGain() / 100.0F + 1.0F);
/*      */     }
/*      */ 
/*      */     public float getFaderGain() {
/*  621 */       return this.faderGain;
/*      */     }
/*      */ 
/*      */     private void initialize()
/*      */     {
/*  626 */       if (this.crossFade != null)
/*  627 */         this.fadeRange = (this.crossFade.volumeAtEnd - this.crossFade.volumeAtStart);
/*      */       else
/*  629 */         this.fadeRange = 0.0F;
/*      */     }
/*      */ 
/*      */     private void pauseFadeMonitor()
/*      */     {
/*  637 */       long wFadePosition = getCrossFadePositionInMilliseconds();
/*      */ 
/*  640 */       synchronized (KJPlayer.this.pauseLock)
/*      */       {
/*      */         try {
/*  643 */           KJPlayer.this.pauseLock.wait();
/*      */         }
/*      */         catch (InterruptedException localInterruptedException)
/*      */         {
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*  651 */       this.crossFadeStartTime = (System.currentTimeMillis() - wFadePosition);
/*      */     }
/*      */ 
/*      */     public void run()
/*      */     {
/*  657 */       synchronized (this.playThread)
/*      */       {
/*  659 */         while (!this.playThread.isStopped())
/*      */         {
/*  662 */           if ((!this.fading) && 
/*  663 */             (this.crossFade != null))
/*      */           {
/*  665 */             if (this.crossFade
/*  665 */               .shouldCrossFade(this.playThread.getPositionInMilliseconds(), 
/*  665 */               this.playThread.getTrack().duration(), KJPlayer.this.vmd.getVolumeThreshold())) {
/*  666 */               startFading();
/*      */             }
/*      */           }
/*      */ 
/*  670 */           if (this.fading)
/*      */           {
/*  672 */             long wFadePosition = getCrossFadePositionInMilliseconds();
/*      */ 
/*  674 */             if (wFadePosition <= this.crossFade.duration)
/*      */             {
/*  677 */               if (this.fadeRange != 0.0F) {
/*  678 */                 this.faderGain = 
/*  679 */                   (this.crossFade.volumeAtStart + 
/*  679 */                   this.fadeRange * ((float)wFadePosition / (float)this.crossFade.duration));
/*      */               }
/*      */ 
/*  683 */               if ((!this.playThread.isEnding()) && 
/*  684 */                 (this.crossFade.startNextTrackAt <= wFadePosition)) {
/*  685 */                 this.playThread.end();
/*      */               }
/*      */ 
/*      */             }
/*  690 */             else if (this.crossFade.stopPlaybackAfterFade()) {
/*  691 */               this.playThread.stop();
/*      */             }
/*      */ 
/*      */           }
/*      */ 
/*  699 */           float wCompositeGain = getCompositeGain();
/*      */ 
/*  702 */           if (this.playThread.getGain() != wCompositeGain) {
/*  703 */             this.playThread.setGain(wCompositeGain);
/*      */           }
/*      */ 
/*      */           try
/*      */           {
/*  708 */             this.playThread.wait(50L);
/*      */           }
/*      */           catch (InterruptedException localInterruptedException)
/*      */           {
/*      */           }
/*      */ 
/*  714 */           if (KJPlayer.this.status == 2) {
/*  715 */             pauseFadeMonitor();
/*      */           }
/*      */ 
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*  724 */       this.playThread = null;
/*      */     }
/*      */ 
/*      */     private void startFading()
/*      */     {
/*  730 */       synchronized (this)
/*      */       {
/*  732 */         if (this.crossFade == null) {
/*  733 */           this.playThread.stop();
/*  734 */           return;
/*      */         }
/*      */ 
/*  737 */         this.crossFadeStartTime = System.currentTimeMillis();
/*      */ 
/*  739 */         if (this.crossFade.startNextTrackAt == 0L) {
/*  740 */           this.playThread.end();
/*      */         }
/*      */ 
/*  743 */         this.fading = true;
/*      */ 
/*  746 */         KJPlayer.this.firePlayerEvent(4, this.playThread.getTrack());
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private class PlayThread implements Runnable {
/*      */     private KJTrack track;
/*      */     private SourceDataLine sourceDataLine;
/*      */     private KJAudioDataProvider audioDataProvider;
/*      */     private KJTrackCrossFade crossFader;
/*  762 */     private boolean ending = false;
/*  763 */     private boolean playTrack = true;
/*      */ 
/*  765 */     private long seekIndex = 0L;
/*      */ 
/*  767 */     private float gain = 100.0F;
/*      */     private KJPlayer.FadeMonitor fadeMonitor;
/*      */ 
/*      */     public PlayThread(KJTrack pTrack) {
/*  772 */       this.track = pTrack;
/*      */ 
/*  774 */       this.audioDataProvider = this.track.getAudioDataProvider(true);
/*  775 */       this.crossFader = this.track.getResolvedCrossFader();
/*      */ 
/*  778 */       this.sourceDataLine = KJPlayer.this.createSourceDataLine(this.track.getAudioDataProvider());
/*      */ 
/*  780 */       prepareFadeMonitor();
/*      */     }
/*      */ 
/*      */     private void cleanUp()
/*      */     {
/*  786 */       synchronized (this)
/*      */       {
/*  788 */         this.track = null;
/*  789 */         this.sourceDataLine = null;
/*      */ 
/*  791 */         this.audioDataProvider = null;
/*  792 */         this.crossFader = null;
/*      */ 
/*  794 */         this.fadeMonitor = null;
/*      */       }
/*      */     }
/*      */ 
/*      */     public void end()
/*      */     {
/*  802 */       this.ending = true;
/*      */ 
/*  804 */       this.track.setState(3);
/*      */ 
/*  806 */       synchronized (KJPlayer.this.playThreadLock) {
/*  807 */         KJPlayer.this.playThreadLock.notify();
/*      */       }
/*      */     }
/*      */ 
/*      */     public KJAudioDataProvider getAudioDataProvider()
/*      */     {
/*  813 */       return this.audioDataProvider;
/*      */     }
/*      */ 
/*      */     public KJTrackCrossFade getCrossFader() {
/*  817 */       return this.crossFader;
/*      */     }
/*      */ 
/*      */     public float getGain() {
/*  821 */       return this.gain;
/*      */     }
/*      */ 
/*      */     public KJPlayer.FadeMonitor getFadeMonitor() {
/*  825 */       return this.fadeMonitor;
/*      */     }
/*      */ 
/*      */     public long getPositionInMilliseconds() {
/*  829 */       return this.sourceDataLine.getMicrosecondPosition() / 1000L - this.seekIndex;
/*      */     }
/*      */ 
/*      */     public SourceDataLine getSourceDataLine() {
/*  833 */       return this.sourceDataLine;
/*      */     }
/*      */ 
/*      */     public KJTrack getTrack() {
/*  837 */       return this.track;
/*      */     }
/*      */ 
/*      */     public boolean isEnding() {
/*  841 */       return this.ending;
/*      */     }
/*      */ 
/*      */     public boolean isStopped() {
/*  845 */       return !this.playTrack;
/*      */     }
/*      */ 
/*      */     public void mute(boolean pState) {
/*  849 */       KJPlayer.this.setSourceDataLineGainPercentage(pState ? 0.0F : this.gain, this.sourceDataLine);
/*      */     }
/*      */ 
/*      */     private void pause(boolean pFlush)
/*      */     {
/*  854 */       if (this.sourceDataLine != null)
/*      */       {
/*  856 */         mute(true);
/*      */ 
/*  858 */         this.sourceDataLine.stop();
/*      */ 
/*  860 */         if (pFlush) {
/*  861 */           this.sourceDataLine.flush();
/*      */         }
/*      */ 
/*  864 */         mute(false);
/*      */       }
/*      */     }
/*      */ 
/*      */     private void prepareFadeMonitor()
/*      */     {
/*  872 */       this.fadeMonitor = new KJPlayer.FadeMonitor(KJPlayer.this, this);
/*      */ 
/*  874 */       this.fadeMonitor.setPriority(1);
/*  875 */       this.fadeMonitor.start();
/*      */ 
/*  878 */       setGain(this.fadeMonitor.getCompositeGain());
/*      */     }
/*      */ 
/*      */     private void unpause()
/*      */     {
/*  883 */       this.sourceDataLine.start();
/*      */     }
/*      */ 
/*      */     public void run()
/*      */     {
/*      */       try
/*      */       {
/*  890 */         KJAudioDataConsumer wSdlAdc = new KJSourceDataLineAudioDataConsumer(
/*  891 */           this.sourceDataLine);
/*      */ 
/*  894 */         KJAudioDataConsumer wAdc = new KJPlayer.CompositeAudioDataConsumer(KJPlayer.this, wSdlAdc, KJPlayer.this.dsp);
/*      */ 
/*  896 */         KJPlayer.this.dsp.start(this.sourceDataLine);
/*      */ 
/*  898 */         this.sourceDataLine.start();
/*      */         do
/*      */         {
/*  906 */           if (!this.ending)
/*  907 */             this.audioDataProvider.writeFrame(wAdc);
/*      */           else {
/*  909 */             this.audioDataProvider.writeFrame(wSdlAdc);
/*      */           }
/*      */ 
/*  913 */           if (KJPlayer.this.status == 2)
/*      */           {
/*  915 */             this.sourceDataLine.flush();
/*      */ 
/*  917 */             synchronized (KJPlayer.this.pauseLock) {
/*      */               try {
/*  919 */                 KJPlayer.this.pauseLock.wait();
/*      */               }
/*      */               catch (InterruptedException localInterruptedException)
/*      */               {
/*      */               }
/*      */             }
/*      */ 
/*  926 */             setGain(this.fadeMonitor.getCompositeGain());
/*      */           }
/*  900 */           if ((!this.audioDataProvider.hasMoreFrames()) || (KJPlayer.this.status == 1)) break; 
/*  900 */         }while (
/*  901 */           this.playTrack);
/*      */       }
/*      */       catch (Exception pEx)
/*      */       {
/*  933 */         this.track.setState(-1);
/*      */       }
/*      */ 
/*  936 */       if ((KJPlayer.this.status != 1) && (this.playTrack))
/*  937 */         this.sourceDataLine.drain();
/*      */       else {
/*  939 */         this.sourceDataLine.flush();
/*      */       }
/*      */ 
/*  942 */       this.sourceDataLine.stop();
/*  943 */       this.sourceDataLine.close();
/*      */ 
/*  945 */       this.audioDataProvider.dispose();
/*      */ 
/*  947 */       this.playTrack = false;
/*      */ 
/*  950 */       synchronized (KJPlayer.this.playThreadLock)
/*      */       {
/*  954 */         if (!this.ending) {
/*  955 */           end();
/*      */         }
/*      */ 
/*  960 */         if (this.track.getState() == 3) {
/*  961 */           this.track.setState(KJPlayer.this.markPlayedTracks ? 2 : 
/*  962 */             0);
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*  969 */       KJPlayer.this.firePlayerEvent(5, this.track);
/*      */ 
/*  971 */       cleanUp();
/*      */     }
/*      */ 
/*      */     public void seek(long pPosition)
/*      */     {
/*  977 */       getAudioDataProvider().seek(pPosition);
/*      */ 
/*  982 */       this.seekIndex = 
/*  983 */         (this.sourceDataLine.getMicrosecondPosition() / 1000L - 
/*  983 */         getAudioDataProvider().positionInMilliseconds());
/*      */ 
/*  985 */       this.sourceDataLine.flush();
/*      */     }
/*      */ 
/*      */     public void setGain(float pPercent)
/*      */     {
/*  990 */       this.gain = pPercent;
/*  991 */       KJPlayer.this.setSourceDataLineGainPercentage(pPercent, this.sourceDataLine);
/*      */     }
/*      */ 
/*      */     public void stop() {
/*  995 */       this.playTrack = false;
/*      */     }
/*      */   }
/*      */ 
/*      */   private static class SingleTrackProvider extends KJAbstractTrackProvider
/*      */   {
/*      */     private KJTrack track;
/*      */ 
/*      */     public SingleTrackProvider(KJTrack pTrack) {
/* 1005 */       this.track = pTrack;
/*      */     }
/*      */ 
/*      */     public KJTrack nextTrack() {
/* 1009 */       return this.track;
/*      */     }
/*      */   }
/*      */ 
/*      */   private class TrackThread extends Thread {
/*      */     private TrackThread() {
/*      */     }
/*      */ 
/*      */     private void prepareAndStartPlayThread(KJTrack pTrack) {
/* 1018 */       KJPlayer.PlayThread wPlayThread = new KJPlayer.PlayThread(KJPlayer.this, pTrack);
/*      */ 
/* 1021 */       Thread wCpl = new Thread(wPlayThread);
/*      */ 
/* 1023 */       wCpl.setPriority(9);
/*      */ 
/* 1026 */       wCpl.start();
/*      */ 
/* 1028 */       KJPlayer.this.setCurrentPlayThread(wPlayThread);
/*      */     }
/*      */ 
/*      */     public void run()
/*      */     {
/* 1034 */       synchronized (KJPlayer.this.playThreadLock)
/*      */       {
/* 1040 */         while ((KJPlayer.this.trackProvider != null) && (KJPlayer.this.status != 1))
/*      */         {
/* 1042 */           KJTrack wTrack = null;
/*      */ 
/* 1044 */           if (KJPlayer.this.nextUserTrack != null)
/*      */           {
/* 1046 */             wTrack = KJPlayer.this.nextUserTrack;
/* 1047 */             KJPlayer.this.nextUserTrack = null;
/*      */ 
/* 1050 */             KJPlayer.this.historyIndex = KJPlayer.this.historyList.size();
/* 1051 */             KJPlayer.this.addToTrackHistory(wTrack);
/*      */           }
/* 1053 */           else if (KJPlayer.this.historyIndex < KJPlayer.this.historyList.size())
/*      */           {
/* 1055 */             wTrack = (KJTrack)KJPlayer.this.historyList.get(KJPlayer.this.historyIndex);
/*      */           }
/*      */           else
/*      */           {
/* 1059 */             wTrack = KJPlayer.this.trackProvider.nextTrack();
/*      */ 
/* 1061 */             if (wTrack == null)
/*      */             {
/*      */               break;
/*      */             }
/* 1065 */             KJPlayer.this.addToTrackHistory(wTrack);
/*      */           }
/*      */ 
/*      */           try
/*      */           {
/* 1072 */             wTrack.setState(1);
/*      */ 
/* 1075 */             prepareAndStartPlayThread(wTrack);
/*      */ 
/* 1078 */             KJPlayer.this.firePlayerEvent(1, wTrack);
/*      */             try
/*      */             {
/* 1083 */               KJPlayer.this.playThreadLock.wait();
/*      */             }
/*      */             catch (InterruptedException localInterruptedException) {
/*      */             }
/*      */           }
/*      */           catch (Exception pEx) {
/* 1089 */             System.err.println("- Error playing track: " + wTrack);
/* 1090 */             pEx.printStackTrace();
/*      */ 
/* 1092 */             wTrack.setState(-1);
/*      */           }
/*      */ 
/* 1096 */           if ((!KJPlayer.this.historyIndexAdjusted) && (KJPlayer.this.historyIndex < KJPlayer.this.historyList.size()))
/* 1097 */             KJPlayer.this.historyIndex += 1;
/*      */           else {
/* 1099 */             KJPlayer.this.historyIndexAdjusted = false;
/*      */           }
/*      */ 
/*      */         }
/*      */ 
/* 1106 */         KJPlayer.this.currentPlayThread = null;
/*      */ 
/* 1108 */         KJPlayer.this.status = 1;
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private class VolumeMonitorDSP
/*      */     implements KJPlayerListener, KJDigitalSignalProcessor
/*      */   {
/* 1119 */     private int vuSampleSize = 256;
/* 1120 */     private int vuHalfSampleSize = this.vuSampleSize >> 1;
/*      */     private float vuAverage;
/*      */     private float vuMaxAverage;
/*      */     private int vuSamples;
/*      */     private KJTrack track;
/*      */     private KJTrackCrossFade crossFader;
/* 1129 */     private float volumeThreshold = 0.0F;
/* 1130 */     private float vuGainPeak = 0.1F;
/*      */ 
/*      */     private VolumeMonitorDSP() {  }
/*      */ 
/* 1133 */     public float getVolumeThreshold() { return this.volumeThreshold;
/*      */     }
/*      */ 
/*      */     public synchronized void process(float[] pLeft, float[] pRight, float pFrameRateRatioHint)
/*      */     {
/* 1141 */       if (((this.crossFader != null) && (!this.crossFader.automatic)) || 
/* 1142 */         (this.track == null) || 
/* 1143 */         (this.track.getAudioDataProvider().positionInMilliseconds() < KJPlayer.this.monitoringDelay))
/*      */       {
/* 1145 */         this.volumeThreshold = 100.0F;
/*      */ 
/* 1147 */         return;
/*      */       }
/*      */ 
/* 1154 */       float wLeft = 0.0F;
/* 1155 */       float wRight = 0.0F;
/*      */ 
/* 1157 */       for (int a = 0; a < pLeft.length; a++)
/*      */       {
/* 1159 */         wLeft += Math.abs(pLeft[a]);
/* 1160 */         wRight += Math.abs(pRight[a]);
/*      */       }
/*      */ 
/* 1164 */       wLeft /= pLeft.length;
/* 1165 */       wRight /= pRight.length;
/*      */ 
/* 1167 */       float vuMax = (wLeft + wRight) / 2.0F;
/*      */ 
/* 1181 */       this.vuAverage += vuMax;
/* 1182 */       this.vuSamples += 1;
/*      */ 
/* 1184 */       if (this.vuSamples > this.vuSampleSize) {
/* 1185 */         this.vuSamples >>= 1;
/* 1186 */         this.vuAverage /= 2.0F;
/*      */       }
/*      */ 
/* 1190 */       if (this.vuSamples >= this.vuHalfSampleSize)
/*      */       {
/* 1192 */         float wVua = this.vuAverage / this.vuSamples;
/*      */ 
/* 1194 */         if (wVua > this.vuMaxAverage) {
/* 1195 */           this.vuMaxAverage = wVua;
/*      */         }
/*      */ 
/* 1198 */         KJJukeBox.getDSPDialog().setAverageVolume(wVua, this.vuMaxAverage);
/*      */ 
/* 1200 */         this.volumeThreshold = (wVua / this.vuMaxAverage * 100.0F);
/*      */       }
/*      */       else
/*      */       {
/* 1206 */         KJJukeBox.getDSPDialog().setAverageVolume(1.0F, 1.0F);
/*      */       }
/*      */     }
/*      */ 
/*      */     public synchronized void playerEvent(KJPlayerEvent pEvent)
/*      */     {
/* 1226 */       if (((pEvent.getType() == 1) && (pEvent.getTrack() != this.track)) || (
/* 1227 */         (pEvent.getType() == 3) && (
/* 1228 */         pEvent.getTrack() == this.track)))
/*      */       {
/* 1230 */         this.track = pEvent.getTrack();
/* 1231 */         if (this.track != null) {
/* 1232 */           this.crossFader = this.track.getResolvedCrossFader();
/*      */ 
/* 1234 */           this.vuAverage = 0.0F;
/* 1235 */           this.vuMaxAverage = 0.0F;
/* 1236 */           this.vuSamples = 0;
/*      */ 
/* 1238 */           this.volumeThreshold = 0.0F;
/*      */ 
/* 1240 */           KJJukeBox.getDSPDialog().setAverageVolume(0.0F, 0.0F);
/*      */         }
/*      */       }
/*      */     }
/*      */   }
/*      */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.player.KJPlayer
 * JD-Core Version:    0.6.0
 */