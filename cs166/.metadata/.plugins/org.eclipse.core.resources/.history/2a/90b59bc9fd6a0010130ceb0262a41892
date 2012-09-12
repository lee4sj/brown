/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.HeadlessException;
/*     */ import java.io.File;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JCheckBox;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JOptionPane;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JProgressBar;
/*     */ import javax.swing.event.ChangeEvent;
/*     */ import javax.swing.event.ChangeListener;
/*     */ import kj.track.KJTrack;
/*     */ import kj.track.KJTrackList;
/*     */ import kj.venue.KJVenue;
/*     */ 
/*     */ public class KJTrackListCleaner extends JDialog
/*     */ {
/*     */   private JButton cleanupButton;
/*     */   private JButton closeButton;
/*     */   private KJTrackList playList;
/*     */   private JProgressBar progressBar;
/*     */   private JCheckBox removeDuplicatesInThisFromOthers;
/*     */   private JCheckBox removeDuplicateTracks;
/*     */   private JCheckBox removeMissing;
/*     */   private KJVenue venue;
/*     */ 
/*     */   public KJTrackListCleaner(JDialog pParent, KJVenue pVenue, KJTrackList pPlayList)
/*     */     throws HeadlessException
/*     */   {
/*  35 */     super(pParent, true);
/*     */ 
/*  37 */     this.venue = pVenue;
/*  38 */     this.playList = pPlayList;
/*     */ 
/*  40 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   private void activateButtons(boolean pState)
/*     */   {
/*  45 */     this.removeMissing.setEnabled(pState);
/*  46 */     this.removeDuplicatesInThisFromOthers.setEnabled(pState);
/*  47 */     this.removeDuplicateTracks.setEnabled(pState);
/*  48 */     this.closeButton.setEnabled(pState);
/*  49 */     this.cleanupButton.setEnabled(pState);
/*     */   }
/*     */ 
/*     */   private void btnCleanup(JButton pButton) {
/*  53 */     boolean wRemoveMissing = this.removeMissing.isSelected();
/*  54 */     boolean wRemoveDuplicateTracks = this.removeDuplicateTracks.isSelected();
/*  55 */     boolean wRemoveDuplicatesInThisFromOthers = this.removeDuplicatesInThisFromOthers.isSelected();
/*     */ 
/*  57 */     if (wRemoveDuplicatesInThisFromOthers) {
/*  58 */       Thread wThread = new RemoveDuplicatesFoundInOtherListsThread(wRemoveMissing, wRemoveDuplicateTracks);
/*  59 */       wThread.start();
/*     */     }
/*  61 */     else if (wRemoveDuplicateTracks) {
/*  62 */       Thread wThread = new RemoveDuplicatesThread(wRemoveMissing);
/*  63 */       wThread.start();
/*     */     }
/*  65 */     else if (wRemoveMissing) {
/*  66 */       Thread wThread = new RemoveMissingThread();
/*  67 */       wThread.start();
/*     */     }
/*     */   }
/*     */ 
/*     */   private void btnClose(JButton pButton)
/*     */   {
/*  74 */     dispose();
/*     */   }
/*     */ 
/*     */   private void initializeValues() {
/*  78 */     this.removeDuplicateTracks.setSelected(true);
/*     */   }
/*     */ 
/*     */   private int performRemoveDuplicates(int pProgressOffset)
/*     */   {
/*  86 */     KJTrack wTrack = null;
/*  87 */     int wRemoved = 0;
/*     */ 
/*  89 */     for (int i = 0; i < this.playList.size(); i++) {
/*  90 */       wTrack = this.playList.getTrack(i);
/*  91 */       for (int k = i + 1; k < this.playList.size(); ) {
/*  92 */         KJTrack wSubTrack = this.playList.getTrack(k);
/*     */ 
/*  94 */         if (wTrack.toString().equals(wSubTrack.toString())) {
/*  95 */           this.playList.removeTrack(k);
/*  96 */           wRemoved++;
/*     */         } else {
/*  98 */           k++;
/*     */         }
/*     */       }
/*     */ 
/* 102 */       setProgress(pProgressOffset + i);
/*     */     }
/*     */ 
/* 105 */     return wRemoved;
/*     */   }
/*     */ 
/*     */   private int performRemoveDuplicatesFoundInOtherLists(int pProgressOffset) {
/* 109 */     int wRemoved = 0;
/*     */ 
/* 111 */     int i = 0; for (int count = 0; i < this.playList.size(); count++) {
/* 112 */       KJTrack wTrack = this.playList.getTrack(i);
/*     */ 
/* 114 */       int j = 0;
/* 115 */       for (j = 0; j < this.venue.getTrackListCount(); j++)
/*     */       {
/* 117 */         KJTrackList wSubPlayList = this.venue.getTrackList(j);
/* 118 */         if (wSubPlayList.equals(this.playList))
/*     */         {
/*     */           continue;
/*     */         }
/* 122 */         int k = 0;
/* 123 */         for (k = 0; k < wSubPlayList.size(); k++) {
/* 124 */           KJTrack wSubTrack = wSubPlayList.getTrack(k);
/*     */ 
/* 126 */           if (wTrack.toString().equals(wSubTrack.toString()))
/*     */           {
/*     */             break;
/*     */           }
/*     */         }
/* 131 */         if (k != wSubPlayList.size())
/*     */         {
/*     */           break;
/*     */         }
/*     */       }
/*     */ 
/* 137 */       if (j == this.venue.getTrackListCount()) {
/* 138 */         i++;
/*     */       } else {
/* 140 */         this.playList.removeTrack(i);
/* 141 */         wRemoved++;
/*     */       }
/*     */ 
/* 144 */       setProgress(pProgressOffset + count);
/*     */     }
/*     */ 
/* 147 */     return wRemoved;
/*     */   }
/*     */ 
/*     */   private int performRemoveMissing()
/*     */   {
/* 155 */     KJTrack wTrack = null;
/* 156 */     int i = 0;
/* 157 */     int wRemoved = 0;
/* 158 */     int wSize = this.playList.size();
/*     */ 
/* 160 */     for (int j = 0; j < wSize; ) {
/* 161 */       wTrack = this.playList.getTrack(i);
/* 162 */       File wTrackFile = wTrack.file();
/* 163 */       if ((wTrackFile == null) || (!wTrackFile.exists())) {
/* 164 */         this.playList.removeTrack(i);
/* 165 */         wRemoved++;
/*     */       } else {
/* 167 */         i++;
/*     */       }
/* 169 */       j++;
/* 170 */       setProgress(j);
/*     */     }
/*     */ 
/* 173 */     setProgress(wSize);
/*     */ 
/* 175 */     return wRemoved;
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/* 180 */     String wTitle = "KJ - Track Cleaner";
/*     */ 
/* 182 */     wTitle = wTitle + " [" + this.playList.toString() + "]";
/* 183 */     setTitle(wTitle);
/*     */ 
/* 185 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/* 187 */     JPanel wOptions = new JPanel(new BorderLayout());
/*     */ 
/* 189 */     getContentPane().add(wOptions, "Center");
/*     */ 
/* 192 */     JPanel wCleanUpOptions = new JPanel(new BorderLayout());
/*     */ 
/* 194 */     wOptions.add(wCleanUpOptions, "Center");
/*     */ 
/* 196 */     wCleanUpOptions.setBorder(KJUITools.createTitleBorder("CleanUp Options"));
/*     */ 
/* 198 */     JPanel wCfWest = new JPanel(new KJFlowLayout());
/* 199 */     wCleanUpOptions.add(wCfWest, "North");
/*     */ 
/* 201 */     StateChangedListener wStateChangedListener = new StateChangedListener(null);
/*     */ 
/* 203 */     this.removeMissing = new JCheckBox("Remove Missing Tracks (no file found)");
/* 204 */     this.removeMissing.setSelected(true);
/* 205 */     this.removeMissing.addChangeListener(wStateChangedListener);
/*     */ 
/* 207 */     this.removeDuplicateTracks = new JCheckBox("Remove Duplicate Tracks");
/* 208 */     this.removeDuplicateTracks.setSelected(false);
/* 209 */     this.removeDuplicateTracks.addChangeListener(wStateChangedListener);
/*     */ 
/* 211 */     this.removeDuplicatesInThisFromOthers = new JCheckBox("Remove (from this PlayList) Tracks in Other PlayLists");
/* 212 */     this.removeDuplicatesInThisFromOthers.setSelected(false);
/* 213 */     this.removeDuplicatesInThisFromOthers.addChangeListener(wStateChangedListener);
/*     */ 
/* 216 */     wCfWest.add(this.removeMissing);
/* 217 */     wCfWest.add(this.removeDuplicateTracks);
/* 218 */     wCfWest.add(this.removeDuplicatesInThisFromOthers);
/*     */ 
/* 220 */     JPanel wBottomPanel = new JPanel();
/* 221 */     wBottomPanel.setLayout(new BorderLayout(5, 5));
/* 222 */     getContentPane().add(wBottomPanel, "South");
/*     */ 
/* 225 */     JPanel wProgressPanel = new JPanel();
/* 226 */     wProgressPanel.setLayout(new BorderLayout());
/* 227 */     wBottomPanel.add(wProgressPanel, "Center");
/*     */ 
/* 229 */     this.progressBar = new JProgressBar();
/* 230 */     wProgressPanel.add(this.progressBar, "Center");
/* 231 */     this.progressBar.setStringPainted(true);
/*     */ 
/* 233 */     this.cleanupButton = KJUITools.createButton("Cleanup", this);
/* 234 */     wProgressPanel.add(this.cleanupButton, "East");
/*     */ 
/* 238 */     JPanel wButtons = new JPanel();
/* 239 */     wButtons.setLayout(new BorderLayout());
/*     */ 
/* 241 */     wBottomPanel.add(wButtons, "South");
/*     */ 
/* 243 */     this.closeButton = KJUITools.createButton("Close", this);
/* 244 */     wButtons.add(this.closeButton, "East");
/*     */ 
/* 246 */     initializeValues();
/*     */ 
/* 248 */     pack();
/*     */ 
/* 250 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*     */ 
/* 252 */     setVisible(true);
/*     */   }
/*     */ 
/*     */   private void setProgress(int i)
/*     */   {
/* 257 */     this.progressBar.setValue(i);
/*     */   }
/*     */ 
/*     */   private void setProgressMax(int max) {
/* 261 */     this.progressBar.setMaximum(max);
/*     */   }
/*     */   private class RemoveDuplicatesFoundInOtherListsThread extends Thread {
/*     */     private boolean removeDuplicateTracks;
/*     */     private boolean removeMissingFiles;
/*     */ 
/*     */     public RemoveDuplicatesFoundInOtherListsThread(boolean pRemoveMissingFiles, boolean pRemoveDuplicateTracks) {
/* 270 */       this.removeMissingFiles = pRemoveMissingFiles;
/* 271 */       this.removeDuplicateTracks = pRemoveDuplicateTracks;
/*     */     }
/*     */ 
/*     */     public void run() {
/* 275 */       KJTrack wTrack = null;
/* 276 */       int wRemoved = 0;
/* 277 */       int wSize = KJTrackListCleaner.this.playList.size();
/* 278 */       int wProgressOffset = 0;
/*     */ 
/* 280 */       KJTrackListCleaner.this.activateButtons(false);
/*     */ 
/* 283 */       KJTrackListCleaner.this.setProgress(0);
/* 284 */       if (this.removeMissingFiles) KJTrackListCleaner.this.setProgressMax(KJTrackListCleaner.this.progressBar.getMaximum() + wSize);
/* 285 */       if (this.removeDuplicateTracks) KJTrackListCleaner.this.setProgressMax(KJTrackListCleaner.this.progressBar.getMaximum() + wSize);
/*     */ 
/* 288 */       if (this.removeMissingFiles) {
/* 289 */         wRemoved = KJTrackListCleaner.this.performRemoveMissing();
/*     */ 
/* 291 */         wProgressOffset = wSize;
/* 292 */         KJTrackListCleaner.this.setProgress(wProgressOffset);
/*     */       }
/*     */ 
/* 295 */       if (this.removeDuplicateTracks) {
/* 296 */         wRemoved += KJTrackListCleaner.this.performRemoveDuplicates(wProgressOffset);
/* 297 */         wProgressOffset += wSize;
/* 298 */         KJTrackListCleaner.this.setProgress(wProgressOffset);
/*     */       }
/*     */ 
/* 301 */       wRemoved += KJTrackListCleaner.this.performRemoveDuplicatesFoundInOtherLists(wProgressOffset);
/*     */ 
/* 303 */       KJTrackListCleaner.this.setProgress(KJTrackListCleaner.this.progressBar.getMaximum());
/*     */ 
/* 305 */       if (wRemoved == 0)
/* 306 */         JOptionPane.showMessageDialog(KJTrackListCleaner.this, "No tracks removed.");
/*     */       else {
/* 308 */         JOptionPane.showMessageDialog(KJTrackListCleaner.this, wRemoved + " track(s) removed.");
/*     */       }
/*     */ 
/* 311 */       KJTrackListCleaner.this.activateButtons(true);
/*     */     }
/*     */   }
/*     */ 
/*     */   private class RemoveDuplicatesThread extends Thread {
/*     */     private boolean removeMissingFiles;
/*     */ 
/*     */     public RemoveDuplicatesThread(boolean pRemoveMissingFiles) {
/* 320 */       this.removeMissingFiles = pRemoveMissingFiles;
/*     */     }
/*     */ 
/*     */     public void run() {
/* 324 */       int wRemoved = 0;
/* 325 */       int wSize = KJTrackListCleaner.this.playList.size();
/* 326 */       int wProgressOffset = 0;
/*     */ 
/* 328 */       KJTrackListCleaner.this.activateButtons(false);
/*     */ 
/* 331 */       KJTrackListCleaner.this.setProgress(0);
/* 332 */       KJTrackListCleaner.this.setProgressMax(wSize);
/* 333 */       if (this.removeMissingFiles) KJTrackListCleaner.this.setProgressMax(KJTrackListCleaner.this.progressBar.getMaximum() + wSize);
/*     */ 
/* 336 */       if (this.removeMissingFiles) {
/* 337 */         wRemoved = KJTrackListCleaner.this.performRemoveMissing();
/*     */ 
/* 339 */         wProgressOffset = wSize;
/* 340 */         KJTrackListCleaner.this.setProgress(wProgressOffset);
/*     */       }
/*     */ 
/* 343 */       wRemoved += KJTrackListCleaner.this.performRemoveDuplicates(wProgressOffset);
/*     */ 
/* 345 */       KJTrackListCleaner.this.setProgress(KJTrackListCleaner.this.progressBar.getMaximum());
/*     */ 
/* 347 */       if (wRemoved == 0)
/* 348 */         JOptionPane.showMessageDialog(KJTrackListCleaner.this, "No tracks removed.");
/*     */       else {
/* 350 */         JOptionPane.showMessageDialog(KJTrackListCleaner.this, wRemoved + " track(s) removed.");
/*     */       }
/*     */ 
/* 353 */       KJTrackListCleaner.this.activateButtons(true);
/*     */     }
/*     */   }
/*     */ 
/*     */   private class RemoveMissingThread extends Thread {
/*     */     public RemoveMissingThread() {
/*     */     }
/*     */ 
/*     */     public void run() {
/* 362 */       int wRemoved = 0;
/* 363 */       int wSize = KJTrackListCleaner.this.playList.size();
/*     */ 
/* 365 */       KJTrackListCleaner.this.activateButtons(false);
/*     */ 
/* 368 */       KJTrackListCleaner.this.setProgress(0);
/* 369 */       KJTrackListCleaner.this.setProgressMax(wSize);
/*     */ 
/* 372 */       wRemoved = KJTrackListCleaner.this.performRemoveMissing();
/*     */ 
/* 374 */       KJTrackListCleaner.this.setProgress(KJTrackListCleaner.this.progressBar.getMaximum());
/*     */ 
/* 376 */       if (wRemoved == 0)
/* 377 */         JOptionPane.showMessageDialog(KJTrackListCleaner.this, "No tracks removed.");
/*     */       else {
/* 379 */         JOptionPane.showMessageDialog(KJTrackListCleaner.this, wRemoved + " track(s) removed.");
/*     */       }
/*     */ 
/* 382 */       KJTrackListCleaner.this.activateButtons(true);
/*     */     }
/*     */   }
/*     */ 
/*     */   private class StateChangedListener implements ChangeListener {
/*     */     private StateChangedListener() {
/*     */     }
/*     */ 
/*     */     public void stateChanged(ChangeEvent e) {
/* 390 */       boolean wEnabled = false;
/*     */ 
/* 392 */       wEnabled |= KJTrackListCleaner.this.removeMissing.isSelected();
/* 393 */       wEnabled |= KJTrackListCleaner.this.removeDuplicateTracks.isSelected();
/* 394 */       wEnabled |= KJTrackListCleaner.this.removeMissing.isSelected();
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJTrackListCleaner
 * JD-Core Version:    0.6.0
 */