/*     */ package kj.track;
/*     */ 
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public class KJTrackCrossFade
/*     */   implements KJXmlSerializable
/*     */ {
/*     */   public boolean automatic;
/*     */   public float automaticVolumeThreshold;
/*     */   public long automaticPositionThreshold;
/*     */   public long duration;
/*     */   public long position;
/*     */   public float volumeAtStart;
/*     */   public float volumeAtEnd;
/*  34 */   public long startNextTrackAt = 0L;
/*     */ 
/*     */   public KJTrackCrossFade()
/*     */   {
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade(long pPositionFromEnd) {
/*  41 */     this(false, -pPositionFromEnd, pPositionFromEnd, 100.0F, 100.0F, 0L, 0.0F, 0L);
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade(long pDuration, float pVolumeAtEnd) {
/*  45 */     this(false, -pDuration, pDuration, 100.0F, pVolumeAtEnd, 0L, 0.0F, 0L);
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade(boolean pAutomatic, long pPosition, long pDuration, float pVolumeAtStart, float pVolumeAtEnd, long pStartNextTrackAt, float pAutomaticVolumeThreshold, long pAutomaticPositionThreshold)
/*     */   {
/*  50 */     this.automatic = pAutomatic;
/*     */ 
/*  52 */     this.position = pPosition;
/*  53 */     this.duration = pDuration;
/*     */ 
/*  55 */     this.volumeAtStart = pVolumeAtStart;
/*  56 */     this.volumeAtEnd = pVolumeAtEnd;
/*     */ 
/*  58 */     this.startNextTrackAt = pStartNextTrackAt;
/*     */ 
/*  60 */     this.automaticVolumeThreshold = pAutomaticVolumeThreshold;
/*  61 */     this.automaticPositionThreshold = pAutomaticPositionThreshold;
/*     */   }
/*     */ 
/*     */   public boolean shouldCrossFade(long pTrackPosition, long pTrackDuration, float pVolumeThreshold)
/*     */   {
/*  69 */     if (this.automatic)
/*     */     {
/*  71 */       return ((pTrackPosition >= pTrackDuration + this.position) && (pVolumeThreshold < this.automaticVolumeThreshold)) || 
/*  71 */         (pTrackPosition >= pTrackDuration - this.automaticPositionThreshold);
/*  72 */     }if (this.position < 0L) {
/*  73 */       return pTrackPosition >= pTrackDuration + this.position;
/*     */     }
/*  75 */     return pTrackPosition >= this.position;
/*     */   }
/*     */ 
/*     */   public boolean stopPlaybackAfterFade()
/*     */   {
/*  83 */     return this.position >= 0L;
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource)
/*     */   {
/*  88 */     this.duration = KJXmlToolkit.get(pSource, "Duration", 0);
/*  89 */     this.position = KJXmlToolkit.get(pSource, "Position", 0);
/*  90 */     this.volumeAtStart = KJXmlToolkit.get(pSource, "VolumeAtStart", 100.0F);
/*  91 */     this.volumeAtEnd = KJXmlToolkit.get(pSource, "VolumeAtEnd", 0.0F);
/*  92 */     this.startNextTrackAt = KJXmlToolkit.get(pSource, "StartNextTrackAt", 0);
/*     */ 
/*  94 */     this.automatic = KJXmlToolkit.get(pSource, "Automatic", false);
/*  95 */     this.automaticVolumeThreshold = KJXmlToolkit.get(pSource, "AutomaticVolumeThreshold", 30.0F);
/*  96 */     this.automaticPositionThreshold = KJXmlToolkit.get(pSource, "AutomaticPositionThreshold", 2000L);
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 102 */     KJXmlToolkit.set(pTarget, "Duration", this.duration);
/* 103 */     KJXmlToolkit.set(pTarget, "Position", this.position);
/* 104 */     KJXmlToolkit.set(pTarget, "VolumeAtStart", this.volumeAtStart);
/* 105 */     KJXmlToolkit.set(pTarget, "VolumeAtEnd", this.volumeAtEnd);
/* 106 */     KJXmlToolkit.set(pTarget, "StartNextTrackAt", this.startNextTrackAt);
/*     */ 
/* 108 */     KJXmlToolkit.set(pTarget, "Automatic", this.automatic);
/* 109 */     KJXmlToolkit.set(pTarget, "AutomaticVolumeThreshold", this.automaticVolumeThreshold);
/* 110 */     KJXmlToolkit.set(pTarget, "AutomaticPositionThreshold", this.automaticPositionThreshold);
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.track.KJTrackCrossFade
 * JD-Core Version:    0.6.0
 */