/*     */ package kj.track;
/*     */ 
/*     */ import java.io.File;
/*     */ import java.util.ArrayList;
/*     */ import java.util.Arrays;
/*     */ import java.util.Comparator;
/*     */ import java.util.Iterator;
/*     */ import java.util.Random;
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public class KJTrackList extends KJAbstractTrackProvider
/*     */   implements KJXmlSerializable
/*     */ {
/*     */   private static final long serialVersionUID = 7806293376295182069L;
/*     */   public static final int PROVIDER_MODE_SEQUENCIAL = 1;
/*     */   public static final int PROVIDER_MODE_RANDOM = 2;
/*  34 */   private static final Random RANDOM = new Random(System.currentTimeMillis());
/*     */ 
/*  36 */   private ArrayList tracks = new ArrayList();
/*     */   private String name;
/*  40 */   private int providingMode = 2;
/*     */ 
/*  42 */   private int index = 0;
/*     */ 
/*  52 */   private KJTrackCrossFade crossFade = new KJTrackCrossFade(true, 
/*  46 */     -15000L, 
/*  47 */     15000L, 
/*  48 */     100.0F, 
/*  49 */     0.0F, 
/*  50 */     0L, 
/*  51 */     40.0F, 
/*  52 */     1000L);
/*     */ 
/*  54 */   private boolean crossFadingEnabled = true;
/*     */ 
/*  56 */   private boolean allowActivation = true;
/*  57 */   private boolean allowDuplicates = true;
/*  58 */   private boolean allowRequests = true;
/*     */ 
/*  60 */   private boolean active = false;
/*     */ 
/*     */   public KJTrackList() {
/*     */   }
/*     */   public KJTrackList(boolean pAllowDuplicates) {
/*  65 */     this.allowDuplicates = pAllowDuplicates;
/*     */   }
/*     */ 
/*     */   public KJTrackList(String pName) {
/*  69 */     this.name = pName;
/*     */   }
/*     */ 
/*     */   public synchronized void addTrack(KJTrack pTrack) {
/*  73 */     addTracks(new KJTrack[] { pTrack });
/*     */   }
/*     */ 
/*     */   public synchronized void addTracks(KJTrack[] pTracks)
/*     */   {
/*  78 */     for (int a = 0; a < pTracks.length; a++)
/*     */     {
/*  81 */       if ((!this.allowDuplicates) && (this.tracks.indexOf(pTracks[a]) != -1))
/*     */       {
/*     */         continue;
/*     */       }
/*  85 */       this.tracks.add(pTracks[a]);
/*     */     }
/*     */ 
/*  89 */     fireTrackProviderChangeEvent(
/*  90 */       1, 
/*  91 */       pTracks);
/*     */   }
/*     */ 
/*     */   public boolean allowRequests()
/*     */   {
/*  96 */     return this.allowRequests;
/*     */   }
/*     */ 
/*     */   public synchronized void clear()
/*     */   {
/* 101 */     KJTrack[] wRemovedTracks = (KJTrack[])this.tracks.toArray(new KJTrack[this.tracks.size()]);
/*     */ 
/* 103 */     this.tracks.clear();
/*     */ 
/* 105 */     fireTrackProviderChangeEvent(
/* 106 */       2, 
/* 107 */       wRemovedTracks);
/*     */   }
/*     */ 
/*     */   private KJTrack findNextTrack()
/*     */   {
/* 114 */     nextTrackIndex();
/*     */ 
/* 116 */     KJTrack wTrack = getTrack(this.index);
/*     */ 
/* 119 */     if (wTrack.getState() != 0)
/*     */     {
/* 122 */       int wIndex = findUnplayedTrack(this.index);
/*     */ 
/* 125 */       if (wIndex == -1)
/*     */       {
/* 128 */         resetPlayedTracks();
/*     */ 
/* 130 */         wIndex = findUnplayedTrack(this.index);
/*     */ 
/* 133 */         if (wIndex != -1)
/* 134 */           this.index = wIndex;
/*     */       }
/*     */       else
/*     */       {
/* 138 */         this.index = wIndex;
/*     */       }
/*     */ 
/* 141 */       wTrack = getTrack(this.index);
/*     */     }
/*     */ 
/* 145 */     return wTrack;
/*     */   }
/*     */ 
/*     */   private void nextTrackIndex()
/*     */   {
/* 151 */     switch (this.providingMode)
/*     */     {
/*     */     case 2:
/* 155 */       this.index = RANDOM.nextInt(size());
/*     */ 
/* 157 */       break;
/*     */     case 1:
/*     */     default:
/* 162 */       this.index += 1;
/*     */ 
/* 164 */       if (this.index < size()) break;
/* 165 */       this.index = 0;
/*     */     }
/*     */   }
/*     */ 
/*     */   private int findUnplayedTrack(int pIndex)
/*     */   {
/* 175 */     int wFc = 0;
/*     */ 
/* 177 */     while (wFc < size())
/*     */     {
/* 179 */       KJTrack wTrack = getTrack(pIndex);
/*     */ 
/* 181 */       if (wTrack.getState() == 0) {
/* 182 */         return pIndex;
/*     */       }
/*     */ 
/* 185 */       pIndex++;
/*     */ 
/* 187 */       if (pIndex >= size()) {
/* 188 */         pIndex = 0;
/*     */       }
/*     */ 
/* 191 */       wFc++;
/*     */     }
/*     */ 
/* 195 */     return -1;
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade getCrossFade()
/*     */   {
/* 200 */     return this.crossFade;
/*     */   }
/*     */ 
/*     */   public KJTrack getCurrentTrack() {
/* 204 */     return getTrack(getCurrentTrackIndex());
/*     */   }
/*     */ 
/*     */   public int getCurrentTrackIndex() {
/* 208 */     return this.index;
/*     */   }
/*     */ 
/*     */   public String getName() {
/* 212 */     return this.name;
/*     */   }
/*     */ 
/*     */   public int getProvidingMode() {
/* 216 */     return this.providingMode;
/*     */   }
/*     */ 
/*     */   public KJTrack getTrack(int pIndex) {
/*     */     try {
/* 221 */       return (KJTrack)this.tracks.get(pIndex); } catch (IndexOutOfBoundsException pEx) {
/*     */     }
/* 223 */     return null;
/*     */   }
/*     */ 
/*     */   public KJTrack[] getTracks()
/*     */   {
/* 228 */     return (KJTrack[])this.tracks.toArray(new KJTrack[this.tracks.size()]);
/*     */   }
/*     */ 
/*     */   public synchronized void insertTrack(KJTrack pTrack, int pIndex) {
/* 232 */     insertTracks(new KJTrack[] { pTrack }, pIndex);
/*     */   }
/*     */ 
/*     */   public synchronized void insertTracks(KJTrack[] pTracks, int pIndex)
/*     */   {
/* 237 */     for (int a = 0; a < pTracks.length; a++)
/*     */     {
/* 240 */       if ((!this.allowDuplicates) && (this.tracks.indexOf(pTracks[a]) != -1))
/*     */       {
/*     */         continue;
/*     */       }
/* 244 */       this.tracks.add(pIndex + a, pTracks[a]);
/*     */     }
/*     */ 
/* 248 */     fireTrackProviderChangeEvent(
/* 249 */       1, 
/* 250 */       pTracks);
/*     */   }
/*     */ 
/*     */   public boolean isActivatable()
/*     */   {
/* 255 */     return this.allowActivation;
/*     */   }
/*     */ 
/*     */   public boolean isActive() {
/* 259 */     return this.active;
/*     */   }
/*     */ 
/*     */   public boolean isCrossFadingEnabled() {
/* 263 */     return this.crossFadingEnabled;
/*     */   }
/*     */ 
/*     */   public synchronized void moveTracks(int[] pIndices, int pDirection)
/*     */   {
/* 272 */     KJTrack[] wChangedTracks = new KJTrack[pIndices.length];
/*     */ 
/* 274 */     if (pDirection < 0)
/*     */     {
/* 276 */       int wTme = 0;
/*     */ 
/* 278 */       for (int a = 0; a < pIndices.length; a++)
/*     */       {
/* 280 */         KJTrack wTrack = (KJTrack)this.tracks.remove(pIndices[a]);
/*     */ 
/* 282 */         wChangedTracks[a] = wTrack;
/*     */ 
/* 284 */         int wNp = pIndices[a] + pDirection;
/*     */ 
/* 286 */         if (wNp < wTme) {
/* 287 */           wNp = wTme++;
/*     */         }
/*     */ 
/* 290 */         pIndices[a] = wNp;
/*     */ 
/* 292 */         this.tracks.add(wNp, wTrack);
/*     */       }
/*     */ 
/*     */     }
/*     */     else
/*     */     {
/* 298 */       int wBme = size() - 1;
/*     */ 
/* 300 */       for (int a = pIndices.length - 1; a >= 0; a--)
/*     */       {
/* 302 */         KJTrack wTrack = (KJTrack)this.tracks.remove(pIndices[a]);
/*     */ 
/* 304 */         wChangedTracks[a] = wTrack;
/*     */ 
/* 306 */         int wNp = pIndices[a] + pDirection;
/*     */ 
/* 308 */         if (wNp > wBme) {
/* 309 */           wNp = wBme--;
/*     */         }
/*     */ 
/* 312 */         pIndices[a] = wNp;
/*     */ 
/* 314 */         this.tracks.add(wNp, wTrack);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 320 */     fireTrackProviderChangeEvent(
/* 321 */       3, 
/* 322 */       wChangedTracks);
/*     */   }
/*     */ 
/*     */   public synchronized KJTrack nextTrack()
/*     */   {
/* 329 */     if (this.tracks.isEmpty()) {
/* 330 */       return null;
/*     */     }
/*     */ 
/* 333 */     return findNextTrack();
/*     */   }
/*     */ 
/*     */   public synchronized KJTrack removeTrack(int pIndex)
/*     */   {
/* 338 */     return removeTracks(new int[] { pIndex })[0];
/*     */   }
/*     */ 
/*     */   public synchronized void removeTrack(KJTrack pTrack) {
/* 342 */     removeTracks(new KJTrack[] { pTrack });
/*     */   }
/*     */ 
/*     */   public synchronized KJTrack[] removeTracks(int[] pIndices)
/*     */   {
/* 347 */     KJTrack[] wRemovedTracks = new KJTrack[pIndices.length];
/*     */ 
/* 349 */     int wIo = 0;
/*     */ 
/* 351 */     for (int a = 0; a < pIndices.length; a++) {
/* 352 */       wRemovedTracks[a] = ((KJTrack)this.tracks.remove(pIndices[a] - wIo));
/* 353 */       wIo++;
/*     */     }
/*     */ 
/* 356 */     if (this.index >= size()) {
/* 357 */       this.index = (size() - 1);
/*     */     }
/*     */ 
/* 360 */     fireTrackProviderChangeEvent(
/* 361 */       2, 
/* 362 */       wRemovedTracks);
/*     */ 
/* 364 */     return wRemovedTracks;
/*     */   }
/*     */ 
/*     */   public synchronized void removeTracks(KJTrack[] pTracks)
/*     */   {
/* 370 */     for (int a = 0; a < pTracks.length; a++) {
/* 371 */       this.tracks.remove(pTracks[a]);
/*     */     }
/*     */ 
/* 374 */     if (this.index >= size()) {
/* 375 */       this.index = (size() - 1);
/*     */     }
/*     */ 
/* 379 */     if (pTracks.length > 0)
/* 380 */       fireTrackProviderChangeEvent(
/* 381 */         2, 
/* 382 */         pTracks);
/*     */   }
/*     */ 
/*     */   public synchronized void resetPlayedTracks()
/*     */   {
/* 389 */     for (int a = 0; a < size(); a++)
/*     */     {
/* 391 */       KJTrack wTrack = getTrack(a);
/*     */ 
/* 393 */       if (wTrack.getState() == 2) {
/* 394 */         getTrack(a).setState(0);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 399 */     fireTrackProviderChangedEvent();
/*     */   }
/*     */ 
/*     */   public void setActivatable(boolean pState)
/*     */   {
/* 404 */     this.allowActivation = pState;
/*     */   }
/*     */ 
/*     */   public void setActive(boolean pState)
/*     */   {
/* 409 */     this.active = pState;
/*     */ 
/* 413 */     fireTrackProviderPropertyChangeEvent(
/* 414 */       "active", 
/* 415 */       this.active);
/*     */   }
/*     */ 
/*     */   public void setAllowRequests(boolean pState)
/*     */   {
/* 420 */     this.allowRequests = pState;
/*     */   }
/*     */ 
/*     */   public void setCrossFade(KJTrackCrossFade pCrossFade) {
/* 424 */     this.crossFade = pCrossFade;
/*     */   }
/*     */ 
/*     */   public void setCrossFadingEnabled(boolean pState) {
/* 428 */     this.crossFadingEnabled = pState;
/*     */   }
/*     */ 
/*     */   public synchronized void setCurrentTrackIndex(int pIndex) {
/* 432 */     this.index = pIndex;
/*     */   }
/*     */ 
/*     */   public void setName(String pName)
/*     */   {
/* 440 */     this.name = pName;
/*     */   }
/*     */ 
/*     */   public void setProvidingMode(int pProvidingMode) {
/* 444 */     this.providingMode = pProvidingMode;
/*     */   }
/*     */ 
/*     */   public int size() {
/* 448 */     return this.tracks.size();
/*     */   }
/*     */ 
/*     */   public synchronized void sort(boolean pAscending)
/*     */   {
/* 453 */     KJTrack wCurrentTrack = getCurrentTrack();
/*     */ 
/* 455 */     Object[] wTrackArray = this.tracks.toArray();
/*     */ 
/* 457 */     Arrays.sort(wTrackArray, new Comparator(pAscending));
/*     */ 
/* 460 */     for (int a = 0; a < wTrackArray.length; a++)
/*     */     {
/* 462 */       this.tracks.set(a, wTrackArray[a]);
/*     */ 
/* 464 */       if (wTrackArray[a] == wCurrentTrack) {
/* 465 */         this.index = a;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 470 */     fireTrackProviderChangedEvent();
/*     */   }
/*     */ 
/*     */   public String toString()
/*     */   {
/* 475 */     return (this.active ? "*" : " ") + " " + getName();
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource)
/*     */   {
/* 480 */     this.name = KJXmlToolkit.get(pSource, "Name", "");
/* 481 */     this.providingMode = KJXmlToolkit.get(pSource, "ProvidingMode", 2);
/* 482 */     this.active = KJXmlToolkit.get(pSource, "Active", true);
/* 483 */     this.index = KJXmlToolkit.get(pSource, "Index", 0);
/* 484 */     this.crossFadingEnabled = KJXmlToolkit.get(pSource, "CrossFadeEnabled", false);
/* 485 */     this.allowActivation = KJXmlToolkit.get(pSource, "AllowActivation", true);
/* 486 */     this.allowDuplicates = KJXmlToolkit.get(pSource, "AllowDuplicates", true);
/* 487 */     this.allowRequests = KJXmlToolkit.get(pSource, "AllowRequests", true);
/*     */ 
/* 489 */     this.crossFade = ((KJTrackCrossFade)KJXmlToolkit.getObject(pSource, "CrossFade"));
/*     */ 
/* 491 */     Iterator wTracks = KJXmlToolkit.getElementsFor(pSource, "Tracks");
/*     */ 
/* 493 */     while (wTracks.hasNext())
/*     */     {
/* 495 */       KJTrack wTrack = (KJTrack)KJXmlToolkit.toObject((Element)wTracks.next());
/*     */ 
/* 497 */       wTrack.setTrackList(this);
/*     */ 
/* 499 */       this.tracks.add(wTrack);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 506 */     KJXmlToolkit.set(pTarget, "Name", this.name);
/* 507 */     KJXmlToolkit.set(pTarget, "ProvidingMode", this.providingMode);
/* 508 */     KJXmlToolkit.set(pTarget, "Active", this.active);
/* 509 */     KJXmlToolkit.set(pTarget, "Index", this.index);
/* 510 */     KJXmlToolkit.set(pTarget, "CrossFadeEnabled", this.crossFadingEnabled);
/* 511 */     KJXmlToolkit.set(pTarget, "AllowActivation", this.allowActivation);
/* 512 */     KJXmlToolkit.set(pTarget, "AllowDuplicates", this.allowDuplicates);
/* 513 */     KJXmlToolkit.set(pTarget, "AllowRequests", this.allowRequests);
/*     */ 
/* 515 */     KJXmlToolkit.setObject(pTarget, "CrossFade", this.crossFade);
/*     */ 
/* 517 */     Element wTracks = KJXmlToolkit.add(pTarget, "Tracks");
/*     */ 
/* 519 */     for (int a = 0; a < this.tracks.size(); a++)
/* 520 */       KJXmlToolkit.setObject(wTracks, "Track", (KJXmlSerializable)this.tracks.get(a));
/*     */   }
/*     */ 
/*     */   private class Comparator implements Comparator
/*     */   {
/*     */     private boolean ascending;
/*     */ 
/*     */     public Comparator(boolean pAscending)
/*     */     {
/* 530 */       this.ascending = pAscending;
/*     */     }
/*     */ 
/*     */     public int compare(Object pTrackA, Object pTrackB)
/*     */     {
/* 535 */       String wTnA = ((KJTrack)pTrackA).file().getName();
/* 536 */       String wTnB = ((KJTrack)pTrackB).file().getName();
/*     */ 
/* 538 */       if (this.ascending) {
/* 539 */         return wTnA.compareToIgnoreCase(wTnB);
/*     */       }
/* 541 */       return -wTnA.compareToIgnoreCase(wTnB);
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.track.KJTrackList
 * JD-Core Version:    0.6.0
 */