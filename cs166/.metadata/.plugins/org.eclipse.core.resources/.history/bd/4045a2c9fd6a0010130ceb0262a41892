/*     */ package kj.venue;
/*     */ 
/*     */ import java.util.ArrayList;
/*     */ import java.util.Iterator;
/*     */ import java.util.Random;
/*     */ import java.util.StringTokenizer;
/*     */ import kj.track.KJAbstractTrackProvider;
/*     */ import kj.track.KJTrack;
/*     */ import kj.track.KJTrackCrossFade;
/*     */ import kj.track.KJTrackList;
/*     */ import kj.track.KJTrackProviderChangeEvent;
/*     */ import kj.track.KJTrackProviderChangeListener;
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public class KJVenue extends KJAbstractTrackProvider
/*     */   implements KJTrackProviderChangeListener, KJXmlSerializable
/*     */ {
/*     */   private static final String VERSION_STRING = "1.0";
/*  35 */   private static final Random RANDOM = new Random();
/*     */   public static final int TRACK_LIST_PICK_MODE_SINGLE = 0;
/*     */   public static final int TRACK_LIST_PICK_MODE_SEQUENCIAL = 1;
/*     */   public static final int TRACK_LIST_PICK_MODE_RANDOM = 2;
/*     */   public static final int TRACK_LIST_PICK_MODE_PROPORTIONAL = 3;
/*     */   public static final int REQUEST_INTERVAL_MODE_NONE = 0;
/*     */   public static final int REQUEST_INTERVAL_MODE_TIME = 1;
/*     */   public static final int REQUEST_INTERVAL_MODE_TRACKS = 2;
/*     */   public static final int REQUEST_INTERVAL_MODE_IMMEDIATE = 3;
/*  47 */   private ArrayList trackLists = new ArrayList();
/*     */ 
/*  49 */   private int trackListPickMode = 0;
/*  50 */   private int activeTrackListIndex = 0;
/*     */ 
/*  52 */   private KJTrackList requestList = new KJTrackList(false);
/*     */   private int requestIntervalCounter;
/*     */   private int requestIntervalNext;
/*  57 */   private int requestIntervalMode = 1;
/*  58 */   private int requestIntervalStart = 5;
/*  59 */   private int requestIntervalEnd = 10;
/*     */   private KJTrack nextRequest;
/*     */   private KJTrack lastTrack;
/*     */   private KJTrack preparedTrack;
/*     */   private String searchQuery;
/*     */   private String[] searchQueryWords;
/*  68 */   private float gain = 100.0F;
/*  69 */   private float microphonePriorityGain = 30.0F;
/*     */ 
/*  71 */   private boolean volumeMonitoringEnabled = true;
/*  72 */   private long volumeMonitoringDelay = 1000L;
/*     */ 
/*  74 */   private boolean navigationCrossFadingEnabled = true;
/*     */ 
/*  76 */   private KJTrackCrossFade navigationCrossFade = new KJTrackCrossFade(
/*  77 */     false, 
/*  78 */     0L, 
/*  79 */     10000L, 
/*  80 */     100.0F, 
/*  81 */     0.0F, 
/*  82 */     5000L, 
/*  83 */     0.0F, 
/*  84 */     0L);
/*     */ 
/*     */   public KJVenue()
/*     */   {
/*  87 */     this(false);
/*     */   }
/*     */ 
/*     */   public KJVenue(boolean pInitializeDefaults)
/*     */   {
/*  92 */     if (pInitializeDefaults) {
/*  93 */       addTrackList(new KJTrackList("<default>"));
/*  94 */       this.requestList = new KJTrackList(false);
/*     */     }
/*     */ 
/*  97 */     initialize();
/*     */   }
/*     */ 
/*     */   public void addTrackList(KJTrackList pPlayList)
/*     */   {
/* 103 */     addTrackListNE(pPlayList);
/*     */ 
/* 105 */     fireTrackProviderChangeEvent(
/* 106 */       1, 
/* 107 */       pPlayList.getTracks());
/*     */   }
/*     */ 
/*     */   protected void addTrackListNE(KJTrackList pTrackList)
/*     */   {
/* 113 */     this.trackLists.add(pTrackList);
/*     */ 
/* 115 */     if (this.trackLists.size() == 1) {
/* 116 */       pTrackList.setActive(true);
/*     */     }
/*     */ 
/* 119 */     pTrackList.addListener(this);
/*     */   }
/*     */ 
/*     */   public void clearPreparedTrack()
/*     */   {
/* 124 */     this.preparedTrack = null;
/*     */   }
/*     */ 
/*     */   public SearchResults find(String pSearchQuery, int pPlayListIndex, int pTrackIndex)
/*     */   {
/* 129 */     this.searchQuery = pSearchQuery;
/* 130 */     this.searchQueryWords = getSearchQueryWords(pSearchQuery);
/*     */ 
/* 132 */     return findNext(pPlayListIndex, pTrackIndex);
/*     */   }
/*     */ 
/*     */   public SearchResults findNext(int pPlayListIndex, int pTrackIndex)
/*     */   {
/* 138 */     if (this.searchQuery != null)
/*     */     {
/* 140 */       int wSpl = pPlayListIndex;
/* 141 */       int wSti = pTrackIndex;
/*     */ 
/* 143 */       boolean wFirstPass = true;
/*     */       while (true)
/*     */       {
/* 147 */         KJTrackList wPlayList = getTrackList(pPlayListIndex);
/*     */ 
/* 149 */         while (pTrackIndex < wPlayList.size())
/*     */         {
/* 151 */           KJTrack wTrack = wPlayList.getTrack(pTrackIndex);
/*     */ 
/* 153 */           if (trackMatchesSearchQuery(wTrack.title().toLowerCase())) {
/* 154 */             return new SearchResults(pPlayListIndex, pTrackIndex);
/*     */           }
/*     */ 
/* 158 */           if ((pPlayListIndex == wSpl) && (pTrackIndex == wSti)) {
/* 159 */             if (wFirstPass)
/* 160 */               wFirstPass = false;
/*     */             else {
/* 162 */               return null;
/*     */             }
/*     */           }
/*     */ 
/* 166 */           pTrackIndex++;
/*     */         }
/*     */ 
/* 170 */         pTrackIndex = 0;
/* 171 */         pPlayListIndex++;
/*     */ 
/* 173 */         if (pPlayListIndex >= getTrackListCount()) {
/* 174 */           pPlayListIndex = 0;
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 181 */     return null;
/*     */   }
/*     */ 
/*     */   public SearchResults findTrack(KJTrack pTrack)
/*     */   {
/* 187 */     for (int a = 0; a < getTrackListCount(); a++)
/*     */     {
/* 189 */       KJTrackList wPlayList = getTrackList(a);
/*     */ 
/* 191 */       for (int b = 0; b < wPlayList.size(); b++)
/*     */       {
/* 193 */         if (wPlayList.getTrack(b) == pTrack) {
/* 194 */           return new SearchResults(a, b);
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 201 */     return null;
/*     */   }
/*     */ 
/*     */   public KJTrackList[] getActiveTrackLists()
/*     */   {
/* 207 */     ArrayList wApll = new ArrayList();
/*     */ 
/* 209 */     for (int a = 0; a < getTrackListCount(); a++)
/*     */     {
/* 211 */       KJTrackList wTrackList = getTrackList(a);
/*     */ 
/* 213 */       if (wTrackList.isActive()) {
/* 214 */         wApll.add(wTrackList);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 219 */     return (KJTrackList[])wApll.toArray(new KJTrackList[wApll.size()]);
/*     */   }
/*     */ 
/*     */   public float getGain()
/*     */   {
/* 228 */     return this.gain;
/*     */   }
/*     */ 
/*     */   public float getMicrophonePriorityGain() {
/* 232 */     return this.microphonePriorityGain;
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade getNavigationCrossFader() {
/* 236 */     return this.navigationCrossFade;
/*     */   }
/*     */ 
/*     */   protected KJTrack getNextRequest()
/*     */   {
/* 241 */     for (int a = 0; a < this.requestList.size(); a++)
/*     */     {
/* 243 */       KJTrack wTrack = this.requestList.getTrack(a);
/*     */ 
/* 247 */       if ((wTrack.getState() != 1) && (wTrack.getState() != 3)) {
/* 248 */         return wTrack;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 253 */     return null;
/*     */   }
/*     */ 
/*     */   public KJTrackList getTrackList(int pIndex)
/*     */   {
/* 258 */     return (KJTrackList)this.trackLists.get(pIndex);
/*     */   }
/*     */ 
/*     */   public KJTrackList getTrackList(String pName)
/*     */   {
/* 263 */     for (int a = 0; a < this.trackLists.size(); a++)
/*     */     {
/* 265 */       KJTrackList wPlayList = getTrackList(a);
/*     */ 
/* 267 */       if (wPlayList.getName().equals(pName)) {
/* 268 */         return wPlayList;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 273 */     return null;
/*     */   }
/*     */ 
/*     */   public int getTrackListCount()
/*     */   {
/* 278 */     return this.trackLists.size();
/*     */   }
/*     */ 
/*     */   public int getTrackListPickMode() {
/* 282 */     return this.trackListPickMode;
/*     */   }
/*     */ 
/*     */   public KJTrack getPreparedTrack() {
/* 286 */     return this.preparedTrack;
/*     */   }
/*     */ 
/*     */   public int getRequestIntervalEnd() {
/* 290 */     return this.requestIntervalEnd;
/*     */   }
/*     */ 
/*     */   public int getRequestIntervalMode() {
/* 294 */     return this.requestIntervalMode;
/*     */   }
/*     */ 
/*     */   public int getRequestIntervalStart() {
/* 298 */     return this.requestIntervalStart;
/*     */   }
/*     */ 
/*     */   public KJTrackList getRequestList() {
/* 302 */     return this.requestList;
/*     */   }
/*     */ 
/*     */   public String getSearchQuery() {
/* 306 */     return this.searchQuery;
/*     */   }
/*     */ 
/*     */   protected String[] getSearchQueryWords(String pSearchQuery)
/*     */   {
/* 311 */     StringTokenizer wSqwt = new StringTokenizer(pSearchQuery, " ");
/*     */ 
/* 313 */     String[] wWords = new String[wSqwt.countTokens()];
/*     */ 
/* 315 */     int wIndex = 0;
/*     */ 
/* 317 */     while (wSqwt.hasMoreTokens()) {
/* 318 */       wWords[(wIndex++)] = wSqwt.nextToken().toLowerCase();
/*     */     }
/*     */ 
/* 321 */     return wWords;
/*     */   }
/*     */ 
/*     */   public long getVolumeMonitoringDelay()
/*     */   {
/* 326 */     return this.volumeMonitoringDelay;
/*     */   }
/*     */ 
/*     */   protected void initialize()
/*     */   {
/* 331 */     this.requestList.addListener(this);
/*     */ 
/* 333 */     this.requestIntervalCounter = -1;
/* 334 */     this.requestIntervalNext = 0;
/*     */ 
/* 336 */     if (this.preparedTrack == null)
/* 337 */       prepareNextTrack();
/*     */   }
/*     */ 
/*     */   public boolean isNavigationCrossFadingEnabled()
/*     */   {
/* 343 */     return this.navigationCrossFadingEnabled;
/*     */   }
/*     */ 
/*     */   public boolean isVolumeMonitoringEnabled() {
/* 347 */     return this.volumeMonitoringEnabled;
/*     */   }
/*     */ 
/*     */   public void removePlayList(KJTrackList pPlayList)
/*     */   {
/* 352 */     int wPi = this.trackLists.indexOf(pPlayList);
/*     */ 
/* 354 */     this.trackLists.remove(wPi);
/*     */ 
/* 361 */     pPlayList.clear();
/*     */ 
/* 363 */     pPlayList.removeListener(this);
/*     */ 
/* 365 */     fireTrackProviderChangeEvent(
/* 366 */       2, 
/* 367 */       pPlayList.getTracks());
/*     */   }
/*     */ 
/*     */   public void setGain(float pGain)
/*     */   {
/* 390 */     this.gain = pGain;
/*     */   }
/*     */ 
/*     */   public void setMicrophonePriorityGain(float pGain) {
/* 394 */     this.microphonePriorityGain = pGain;
/*     */   }
/*     */ 
/*     */   public void setNavigationCrossFader(KJTrackCrossFade pCrossFader) {
/* 398 */     this.navigationCrossFade = pCrossFader;
/*     */   }
/*     */ 
/*     */   public void setNavigationCrossFadingEnabled(boolean pState) {
/* 402 */     this.navigationCrossFadingEnabled = pState;
/*     */   }
/*     */ 
/*     */   public void setRequestIntervalEnd(int pEnd) {
/* 406 */     this.requestIntervalEnd = pEnd;
/*     */   }
/*     */ 
/*     */   public void setRequestIntervalMode(int pMode)
/*     */   {
/* 411 */     int wOldRequestMode = this.requestIntervalMode;
/*     */ 
/* 413 */     this.requestIntervalMode = pMode;
/*     */ 
/* 416 */     if ((pMode != wOldRequestMode) && ((pMode == 0) || (pMode == 3)))
/*     */     {
/* 418 */       clearPreparedTrack();
/* 419 */       prepareNextTrack();
/*     */ 
/* 421 */       fireTrackProviderChangedEvent();
/*     */     }
/*     */   }
/*     */ 
/*     */   public void setRequestIntervalStart(int pStart)
/*     */   {
/* 428 */     this.requestIntervalStart = pStart;
/*     */   }
/*     */ 
/*     */   private void setSingleActiveTrackList(KJTrackList pTrackList)
/*     */   {
/* 433 */     for (int a = 0; a < getTrackListCount(); a++)
/*     */     {
/* 435 */       KJTrackList wTrackList = getTrackList(a);
/*     */ 
/* 437 */       if (wTrackList != pTrackList)
/* 438 */         wTrackList.setActive(false);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void setTrackListPickMode(int pTrackListPickMode)
/*     */   {
/* 446 */     setTrackListPickMode(pTrackListPickMode, null);
/*     */   }
/*     */ 
/*     */   public void setTrackListPickMode(int pTrackListPickMode, KJTrackList pDefaultActiveTrackList)
/*     */   {
/* 451 */     this.trackListPickMode = pTrackListPickMode;
/*     */ 
/* 453 */     if (this.trackListPickMode == 0)
/* 454 */       setSingleActiveTrackList(pDefaultActiveTrackList);
/*     */   }
/*     */ 
/*     */   public void setVolumeMonitoringDelay(long pDelay)
/*     */   {
/* 460 */     this.volumeMonitoringDelay = pDelay;
/*     */   }
/*     */ 
/*     */   public void setVolumeMonitoringEnabled(boolean pState) {
/* 464 */     this.volumeMonitoringEnabled = pState;
/*     */   }
/*     */ 
/*     */   protected boolean shouldProvideRequest()
/*     */   {
/* 469 */     if (this.requestList.size() > 0)
/*     */     {
/* 472 */       if (this.requestIntervalMode == 3) {
/* 473 */         return true;
/*     */       }
/*     */ 
/* 477 */       if (this.requestIntervalCounter == -1) {
/* 478 */         this.requestIntervalNext = (RANDOM.nextInt(this.requestIntervalEnd - this.requestIntervalStart) + this.requestIntervalStart);
/* 479 */         this.requestIntervalCounter = 0;
/* 480 */       } else if (this.requestIntervalCounter >= this.requestIntervalNext) {
/* 481 */         return true;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 486 */     return false;
/*     */   }
/*     */ 
/*     */   protected boolean trackMatchesSearchQuery(String pTrackTitle)
/*     */   {
/* 492 */     int wMatchCount = 0;
/*     */ 
/* 494 */     for (int a = 0; a < this.searchQueryWords.length; a++) {
/* 495 */       if (pTrackTitle.indexOf(this.searchQueryWords[a]) != -1) {
/* 496 */         wMatchCount++;
/*     */       }
/*     */     }
/*     */ 
/* 500 */     return wMatchCount == this.searchQueryWords.length;
/*     */   }
/*     */ 
/*     */   public KJTrack nextTrack()
/*     */   {
/* 506 */     KJTrack wTrack = null;
/*     */ 
/* 508 */     if (this.preparedTrack != null)
/*     */     {
/* 510 */       wTrack = this.preparedTrack;
/* 511 */       this.preparedTrack = null;
/*     */     }
/*     */     else
/*     */     {
/* 516 */       if ((shouldProvideRequest()) && ((this.lastTrack == null) || (this.lastTrack.allowRequestsToFollow())))
/*     */       {
/* 518 */         wTrack = getNextRequest();
/*     */ 
/* 520 */         this.nextRequest = wTrack;
/*     */ 
/* 522 */         this.requestIntervalCounter = -1;
/*     */       }
/*     */ 
/* 528 */       if (wTrack == null)
/*     */       {
/* 530 */         this.nextRequest = null;
/*     */ 
/* 532 */         KJTrackList wApl = pickAnActiveTrackList();
/*     */ 
/* 534 */         if (wApl != null) {
/* 535 */           wTrack = wApl.nextTrack();
/*     */         }
/*     */ 
/* 538 */         updateRequestIntervalCounter(wTrack);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 544 */     this.lastTrack = wTrack;
/*     */ 
/* 546 */     return wTrack;
/*     */   }
/*     */ 
/*     */   protected KJTrackList pickAnActiveTrackList()
/*     */   {
/* 552 */     switch (this.trackListPickMode)
/*     */     {
/*     */     case 0:
/* 556 */       for (int a = 0; a < getTrackListCount(); a++)
/*     */       {
/* 558 */         KJTrackList wTrackList = getTrackList(a);
/*     */ 
/* 560 */         if (wTrackList.isActive()) {
/* 561 */           return wTrackList;
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/* 566 */       return null;
/*     */     case 2:
/* 570 */       this.activeTrackListIndex = RANDOM.nextInt(getTrackListCount());
/*     */ 
/* 572 */       break;
/*     */     case 3:
/* 576 */       int wTotalTracks = 0;
/*     */ 
/* 579 */       for (int a = 0; a < getTrackListCount(); a++) {
/* 580 */         wTotalTracks += getTrackList(a).size();
/*     */       }
/*     */ 
/* 584 */       int wTrackIndex = (int)(RANDOM.nextDouble() * wTotalTracks);
/* 585 */       int wTrackCount = 0;
/*     */ 
/* 588 */       for (int a = 0; a < getTrackListCount(); a++)
/*     */       {
/* 590 */         KJTrackList wTrackList = getTrackList(a);
/*     */ 
/* 592 */         if ((wTrackIndex >= wTrackCount) && (wTrackIndex < wTrackCount + wTrackList.size())) {
/* 593 */           this.activeTrackListIndex = a;
/* 594 */           break;
/*     */         }
/*     */ 
/* 597 */         wTrackCount += wTrackList.size();
/*     */       }
/*     */ 
/*     */     case 1:
/*     */     }
/*     */ 
/* 608 */     int wPickCount = 0;
/*     */ 
/* 610 */     while (wPickCount < getTrackListCount())
/*     */     {
/* 612 */       KJTrackList wTrackList = getTrackList(this.activeTrackListIndex);
/*     */ 
/* 614 */       this.activeTrackListIndex += 1;
/*     */ 
/* 616 */       if (this.activeTrackListIndex >= getTrackListCount()) {
/* 617 */         this.activeTrackListIndex = 0;
/*     */       }
/*     */ 
/* 620 */       if (wTrackList.isActive()) {
/* 621 */         return wTrackList;
/*     */       }
/*     */ 
/* 624 */       wPickCount++;
/*     */     }
/*     */ 
/* 628 */     return null;
/*     */   }
/*     */ 
/*     */   public void prepareNextTrack()
/*     */   {
/* 633 */     this.preparedTrack = nextTrack();
/*     */   }
/*     */ 
/*     */   public void trackProviderChanged(KJTrackProviderChangeEvent pEvent)
/*     */   {
/* 639 */     if (pEvent.isPropertyChange())
/*     */     {
/* 643 */       if ((this.trackListPickMode == 0) && 
/* 644 */         (pEvent.getPropertyName() == "active") && (pEvent.getPropertyValue() == Boolean.TRUE))
/*     */       {
/* 648 */         setSingleActiveTrackList((KJTrackList)pEvent.getSource());
/*     */       }
/*     */ 
/* 653 */       clearPreparedTrack();
/* 654 */       prepareNextTrack();
/*     */ 
/* 657 */       fireTrackProviderChangedEvent();
/*     */     }
/* 662 */     else if (pEvent.getSource() == this.requestList)
/*     */     {
/* 664 */       boolean wRequestedState = true;
/*     */ 
/* 666 */       if (pEvent.getType() == 2) {
/* 667 */         wRequestedState = false;
/*     */       }
/*     */ 
/* 670 */       boolean wPrepareNext = false;
/*     */ 
/* 675 */       for (int a = 0; a < pEvent.getTracks().length; a++)
/*     */       {
/* 677 */         KJTrack wTrack = pEvent.getTracks()[a];
/*     */ 
/* 682 */         if (wTrack == this.nextRequest) {
/* 683 */           wPrepareNext = true;
/*     */         }
/*     */ 
/* 686 */         wTrack.setRequested(wRequestedState);
/*     */       }
/*     */ 
/* 694 */       if (wPrepareNext) {
/* 695 */         clearPreparedTrack();
/* 696 */         prepareNextTrack();
/*     */       }
/*     */ 
/* 699 */       fireTrackProviderChangeEvent(
/* 700 */         3, 
/* 701 */         pEvent.getTracks());
/*     */     }
/*     */     else
/*     */     {
/* 705 */       if (pEvent.getType() == 2)
/*     */       {
/* 707 */         getRequestList().removeTracks(pEvent.getTracks());
/*     */ 
/* 710 */         if (this.preparedTrack != null)
/*     */         {
/* 712 */           for (int a = 0; a < pEvent.getTracks().length; a++)
/*     */           {
/* 714 */             if (this.preparedTrack == pEvent.getTracks()[a]) {
/* 715 */               this.preparedTrack = null;
/* 716 */               prepareNextTrack();
/*     */             }
/*     */ 
/*     */           }
/*     */ 
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/* 725 */       fireTrackProviderChangeEvent(pEvent.getType(), pEvent.getTracks());
/*     */     }
/*     */   }
/*     */ 
/*     */   protected void updateRequestIntervalCounter(KJTrack pTrack)
/*     */   {
/* 735 */     if (pTrack == null) {
/* 736 */       return;
/*     */     }
/*     */ 
/* 739 */     if (this.requestList.size() > 0)
/*     */     {
/* 741 */       switch (this.requestIntervalMode)
/*     */       {
/*     */       case 1:
/* 745 */         this.requestIntervalCounter = (int)(this.requestIntervalCounter + pTrack.duration() / 60000L);
/* 746 */         break;
/*     */       case 2:
/* 749 */         this.requestIntervalCounter += 1;
/*     */       default:
/* 749 */         break;
/*     */       }
/*     */ 
/*     */     }
/*     */     else
/*     */     {
/* 758 */       this.requestIntervalCounter = -1;
/*     */     }
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource)
/*     */   {
/* 832 */     Element wVenue = pSource;
/*     */ 
/* 834 */     this.activeTrackListIndex = KJXmlToolkit.get(wVenue, "ActiveTrackListIndex", 0);
/* 835 */     this.requestIntervalCounter = KJXmlToolkit.get(wVenue, "RequestIntervalCounter", 0);
/* 836 */     this.requestIntervalNext = KJXmlToolkit.get(wVenue, "RequestIntervalNext", 0);
/* 837 */     this.requestIntervalMode = KJXmlToolkit.get(wVenue, "RequestIntervalMode", 1);
/* 838 */     this.requestIntervalStart = KJXmlToolkit.get(wVenue, "RequestIntervalStart", 5);
/* 839 */     this.requestIntervalEnd = KJXmlToolkit.get(wVenue, "RequestIntervalEnd", 10);
/* 840 */     this.trackListPickMode = KJXmlToolkit.get(wVenue, "TrackListPickMode", 0);
/* 841 */     this.searchQuery = KJXmlToolkit.get(wVenue, "SearchQuery", "");
/* 842 */     this.gain = KJXmlToolkit.get(wVenue, "Gain", 100.0F);
/* 843 */     this.microphonePriorityGain = KJXmlToolkit.get(wVenue, "MicrophonePriorityGain", 30.0F);
/* 844 */     this.navigationCrossFadingEnabled = KJXmlToolkit.get(wVenue, "NavigationCrossFadingEnabled", true);
/* 845 */     this.volumeMonitoringEnabled = KJXmlToolkit.get(wVenue, "VolumeMonitoringEnabled", true);
/* 846 */     this.volumeMonitoringDelay = KJXmlToolkit.get(wVenue, "VolumeMonitoringEnabled", 2500L);
/*     */ 
/* 851 */     this.navigationCrossFade = ((KJTrackCrossFade)KJXmlToolkit.getObject(wVenue, "NavigationCrossFade"));
/*     */ 
/* 853 */     Iterator wTrackLists = KJXmlToolkit.getElementsFor(wVenue, "TrackLists");
/*     */ 
/* 855 */     while (wTrackLists.hasNext()) {
/* 856 */       addTrackListNE((KJTrackList)KJXmlToolkit.toObject((Element)wTrackLists.next()));
/*     */     }
/*     */ 
/* 859 */     this.requestList = ((KJTrackList)KJXmlToolkit.getObject(wVenue, "RequestList"));
/*     */ 
/* 861 */     initialize();
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 867 */     Element wVenue = pTarget;
/*     */ 
/* 869 */     wVenue.setAttribute("version", "1.0");
/*     */ 
/* 871 */     KJXmlToolkit.set(wVenue, "ActiveTrackListIndex", this.activeTrackListIndex);
/* 872 */     KJXmlToolkit.set(wVenue, "RequestIntervalCounter", this.requestIntervalCounter);
/* 873 */     KJXmlToolkit.set(wVenue, "RequestIntervalNext", this.requestIntervalNext);
/* 874 */     KJXmlToolkit.set(wVenue, "RequestIntervalMode", this.requestIntervalMode);
/* 875 */     KJXmlToolkit.set(wVenue, "RequestIntervalStart", this.requestIntervalStart);
/* 876 */     KJXmlToolkit.set(wVenue, "RequestIntervalEnd", this.requestIntervalEnd);
/* 877 */     KJXmlToolkit.set(wVenue, "TrackListPickMode", this.trackListPickMode);
/* 878 */     KJXmlToolkit.set(wVenue, "SearchQuery", this.searchQuery);
/* 879 */     KJXmlToolkit.set(wVenue, "Gain", this.gain);
/* 880 */     KJXmlToolkit.set(wVenue, "MicrophonePriorityGain", this.microphonePriorityGain);
/* 881 */     KJXmlToolkit.set(wVenue, "NavigationCrossFadingEnabled", this.navigationCrossFadingEnabled);
/* 882 */     KJXmlToolkit.set(wVenue, "VolumeMonitoringEnabled", this.volumeMonitoringEnabled);
/* 883 */     KJXmlToolkit.set(wVenue, "VolumeMonitoringDelay", this.volumeMonitoringDelay);
/*     */ 
/* 888 */     KJXmlToolkit.setObject(wVenue, "NavigationCrossFade", this.navigationCrossFade);
/*     */ 
/* 890 */     Element wTrackLists = KJXmlToolkit.add(wVenue, "TrackLists");
/*     */ 
/* 892 */     for (int a = 0; a < this.trackLists.size(); a++)
/*     */     {
/* 894 */       KJXmlToolkit.setObject(
/* 895 */         wTrackLists, 
/* 896 */         "TrackList", 
/* 897 */         (KJTrackList)this.trackLists.get(a));
/*     */     }
/*     */ 
/* 901 */     KJXmlToolkit.setObject(wVenue, "RequestList", this.requestList);
/*     */   }
/*     */ 
/*     */   public static class SearchResults {
/*     */     public int trackListIndex;
/*     */     public int trackIndex;
/*     */ 
/*     */     public SearchResults(int pPlayListIndex, int pTrackIndex) {
/* 911 */       this.trackListIndex = pPlayListIndex;
/* 912 */       this.trackIndex = pTrackIndex;
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.venue.KJVenue
 * JD-Core Version:    0.6.0
 */