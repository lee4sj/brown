/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.HeadlessException;
/*     */ import java.awt.event.ItemEvent;
/*     */ import java.awt.event.ItemListener;
/*     */ import java.io.File;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JCheckBox;
/*     */ import javax.swing.JComboBox;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import kj.track.KJTrackList;
/*     */ 
/*     */ public class KJTrackListPreferences extends JDialog
/*     */ {
/*     */   private static final String TRACK_PROVIDING_MODE_RANDOM = "Random";
/*     */   private static final String TRACK_PROVIDING_MODE_SEQUENCIAL = "Sequencial";
/*     */   private KJTrackList playList;
/*     */   private JComboBox trackProvidingMode;
/*     */   private JCheckBox trackListActivatable;
/*     */   private JCheckBox allowRequests;
/*     */   private JCheckBox crossFadingEnabled;
/*     */   private KJCrossFaderProperties crossFaderProperties;
/*     */ 
/*     */   public KJTrackListPreferences(JDialog pParent, KJTrackList pPlayList)
/*     */     throws HeadlessException
/*     */   {
/*  48 */     super(pParent, true);
/*     */ 
/*  50 */     this.playList = pPlayList;
/*     */ 
/*  52 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/*  58 */     setTitle("KJ - Track List Preferences [" + this.playList.getName() + "]");
/*  59 */     setSize(512, 512);
/*     */ 
/*  61 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/*  64 */     JPanel wTrackListPreferences = new JPanel(new KJFlowLayout());
/*     */ 
/*  66 */     getContentPane().add(wTrackListPreferences, "Center");
/*     */ 
/*  70 */     JPanel wTrackListOptions = new JPanel(new KJFlowLayout());
/*     */ 
/*  72 */     wTrackListOptions.setBorder(KJUITools.createTitleBorder("Track List Options"));
/*     */ 
/*  74 */     wTrackListPreferences.add(wTrackListOptions);
/*     */ 
/*  76 */     JPanel wTrackProvidingModePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/*  78 */     wTrackListOptions.add(wTrackProvidingModePanel);
/*     */ 
/*  80 */     wTrackProvidingModePanel.add(new JLabel("Play Mode: "));
/*     */ 
/*  82 */     this.trackProvidingMode = 
/*  85 */       new JComboBox(new String[] { 
/*  84 */       "Random", 
/*  85 */       "Sequencial" });
/*     */ 
/*  89 */     this.trackProvidingMode.setPreferredSize(new Dimension(128, 17));
/*     */ 
/*  91 */     wTrackProvidingModePanel.add(this.trackProvidingMode);
/*     */ 
/*  93 */     this.trackListActivatable = new JCheckBox("Allow track list to be activated.");
/*     */ 
/*  95 */     wTrackListOptions.add(this.trackListActivatable);
/*     */ 
/*  97 */     this.allowRequests = new JCheckBox("Allow requests when track list is active.");
/*     */ 
/*  99 */     wTrackListOptions.add(this.allowRequests, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/* 102 */     JPanel wCrossFadingPanel = new JPanel(new BorderLayout());
/*     */ 
/* 104 */     wTrackListPreferences.add(wCrossFadingPanel, KJFlowLayout.CONSTRAINT_NO_INDENT);
/*     */ 
/* 106 */     wCrossFadingPanel.setBorder(KJUITools.createTitleBorder("Cross Fader"));
/*     */ 
/* 108 */     this.crossFadingEnabled = new JCheckBox("Enabled");
/*     */ 
/* 110 */     this.crossFadingEnabled.addItemListener(
/* 111 */       new ItemListener() {
/*     */       public void itemStateChanged(ItemEvent pEvent) {
/* 113 */         KJUITools.setEnabled(
/* 114 */           KJTrackListPreferences.this.crossFaderProperties, 
/* 115 */           pEvent.getStateChange() == 1);
/*     */       }
/*     */     });
/* 120 */     wCrossFadingPanel.add(this.crossFadingEnabled, "North");
/*     */ 
/* 122 */     this.crossFaderProperties = new KJCrossFaderProperties();
/*     */ 
/* 124 */     wCrossFadingPanel.add(this.crossFaderProperties, "Center");
/*     */ 
/* 126 */     KJUITools.setEnabled(this.crossFaderProperties, false);
/*     */ 
/* 189 */     JPanel wButtons = new JPanel();
/*     */ 
/* 191 */     getContentPane().add(wButtons, "South");
/*     */ 
/* 193 */     wButtons.add(KJUITools.createButton("Ok", this));
/* 194 */     wButtons.add(KJUITools.createButton("Cancel", this));
/*     */ 
/* 196 */     initializeValues();
/*     */ 
/* 198 */     pack();
/*     */ 
/* 200 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*     */ 
/* 202 */     setVisible(true);
/*     */   }
/*     */ 
/*     */   private void initializeValues()
/*     */   {
/* 208 */     if (this.playList.getProvidingMode() == 2)
/* 209 */       this.trackProvidingMode.setSelectedItem("Random");
/*     */     else {
/* 211 */       this.trackProvidingMode.setSelectedItem("Sequencial");
/*     */     }
/*     */ 
/* 214 */     this.trackListActivatable.setSelected(this.playList.isActivatable());
/* 215 */     this.allowRequests.setSelected(this.playList.allowRequests());
/*     */ 
/* 217 */     this.crossFadingEnabled.setSelected(this.playList.isCrossFadingEnabled());
/*     */ 
/* 219 */     if (this.playList.isCrossFadingEnabled())
/* 220 */       this.crossFaderProperties.setCrossFader(this.playList.getCrossFade());
/*     */   }
/*     */ 
/*     */   private boolean isFileNameExtension(File pFile, String pExtension)
/*     */   {
/* 227 */     String wFileName = pFile.getName();
/*     */ 
/* 229 */     int wEi = wFileName.lastIndexOf('.');
/*     */ 
/* 231 */     if (wEi != -1) {
/* 232 */       return wFileName.substring(wEi + 1).equalsIgnoreCase(pExtension);
/*     */     }
/*     */ 
/* 235 */     return false;
/*     */   }
/*     */ 
/*     */   private void btnCancel(JButton pButton)
/*     */   {
/* 240 */     dispose();
/*     */   }
/*     */ 
/*     */   private void btnOk(JButton pButton)
/*     */   {
/* 245 */     if (this.trackProvidingMode.getSelectedItem() == "Random")
/* 246 */       this.playList.setProvidingMode(2);
/*     */     else {
/* 248 */       this.playList.setProvidingMode(1);
/*     */     }
/*     */ 
/* 251 */     this.playList.setActivatable(this.trackListActivatable.isSelected());
/* 252 */     this.playList.setAllowRequests(this.allowRequests.isSelected());
/*     */ 
/* 254 */     this.playList.setCrossFadingEnabled(this.crossFadingEnabled.isSelected());
/*     */ 
/* 256 */     if (this.crossFadingEnabled.isSelected()) {
/* 257 */       this.playList.setCrossFade(this.crossFaderProperties.getCrossFader());
/*     */     }
/*     */ 
/* 260 */     dispose();
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJTrackListPreferences
 * JD-Core Version:    0.6.0
 */