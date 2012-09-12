/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.HeadlessException;
/*     */ import java.io.File;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JCheckBox;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JFrame;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JTextField;
/*     */ import kj.venue.KJVenue;
/*     */ 
/*     */ public class KJVenuePreferrences extends JDialog
/*     */ {
/*     */   private KJVenue venue;
/*     */   private JTextField microphonePriorityVolume;
/*     */   private JTextField monitoringDelay;
/*     */   private JCheckBox disableVolumeMonitoring;
/*     */   private JCheckBox crossFadingEnabled;
/*     */   private KJCrossFaderProperties crossFaderProperties;
/*     */ 
/*     */   public KJVenuePreferrences(JFrame pParent, KJVenue pVenue)
/*     */     throws HeadlessException
/*     */   {
/*  45 */     super(pParent, true);
/*     */ 
/*  47 */     this.venue = pVenue;
/*     */ 
/*  49 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/*  55 */     setTitle("KJ - Venue Preferences");
/*  56 */     setSize(512, 512);
/*     */ 
/*  58 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/*  61 */     JPanel wVenuePreferrences = new JPanel(new KJFlowLayout());
/*     */ 
/*  63 */     getContentPane().add(wVenuePreferrences, "Center");
/*     */ 
/*  65 */     wVenuePreferrences.setBorder(KJUITools.createTitleBorder("Venue Preferrences"));
/*     */ 
/*  67 */     JPanel wVolumnOptionsPanel = new JPanel(new KJFlowLayout());
/*     */ 
/*  69 */     wVenuePreferrences.add(wVolumnOptionsPanel);
/*     */ 
/*  71 */     wVolumnOptionsPanel.setBorder(KJUITools.createTitleBorder("Volume Options"));
/*     */ 
/*  74 */     JPanel wMicrophonePriorityVolumePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/*  76 */     wVolumnOptionsPanel.add(wMicrophonePriorityVolumePanel);
/*     */ 
/*  78 */     wMicrophonePriorityVolumePanel.add(new JLabel("- Microphone priority volume: "));
/*     */ 
/*  80 */     this.microphonePriorityVolume = new JTextField();
/*  81 */     this.microphonePriorityVolume.setHorizontalAlignment(4);
/*  82 */     this.microphonePriorityVolume.setPreferredSize(new Dimension(48, 17));
/*     */ 
/*  84 */     wMicrophonePriorityVolumePanel.add(this.microphonePriorityVolume);
/*     */ 
/*  86 */     wMicrophonePriorityVolumePanel.add(new JLabel("%"));
/*     */ 
/*  89 */     this.disableVolumeMonitoring = new JCheckBox("Disable volume montioring so KJ uses less CPU.");
/*     */ 
/*  91 */     wVolumnOptionsPanel.add(this.disableVolumeMonitoring, KJFlowLayout.CONSTRAINT_SEPARATOR);
/*     */ 
/*  94 */     JPanel wVolumeMontioringDelayPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/*  96 */     wVolumnOptionsPanel.add(wVolumeMontioringDelayPanel, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/*  98 */     wVolumeMontioringDelayPanel.add(new JLabel("- Monitor volume "));
/*     */ 
/* 100 */     this.monitoringDelay = new JTextField();
/* 101 */     this.monitoringDelay.setHorizontalAlignment(4);
/* 102 */     this.monitoringDelay.setPreferredSize(new Dimension(48, 17));
/*     */ 
/* 104 */     wVolumeMontioringDelayPanel.add(this.monitoringDelay);
/*     */ 
/* 106 */     wVolumeMontioringDelayPanel.add(new JLabel(" after track start."));
/*     */ 
/* 109 */     JPanel wCrossFadingPanel = new JPanel(new KJFlowLayout());
/*     */ 
/* 111 */     wCrossFadingPanel.setBorder(KJUITools.createTitleBorder("Navigation Cross Fader"));
/*     */ 
/* 113 */     wVenuePreferrences.add(wCrossFadingPanel);
/*     */ 
/* 115 */     this.crossFadingEnabled = new JCheckBox("Cross-fade when navigating.");
/*     */ 
/* 117 */     wCrossFadingPanel.add(this.crossFadingEnabled);
/*     */ 
/* 119 */     this.crossFaderProperties = new KJCrossFaderProperties(true);
/*     */ 
/* 121 */     wCrossFadingPanel.add(this.crossFaderProperties);
/*     */ 
/* 124 */     JPanel wButtons = new JPanel();
/*     */ 
/* 126 */     getContentPane().add(wButtons, "South");
/*     */ 
/* 128 */     wButtons.add(KJUITools.createButton("Ok", this));
/* 129 */     wButtons.add(KJUITools.createButton("Cancel", this));
/*     */ 
/* 131 */     initializeValues();
/*     */ 
/* 133 */     pack();
/*     */ 
/* 135 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*     */ 
/* 137 */     setVisible(true);
/*     */   }
/*     */ 
/*     */   private void initializeValues()
/*     */   {
/* 143 */     this.microphonePriorityVolume.setText(String.valueOf(this.venue.getMicrophonePriorityGain()));
/*     */ 
/* 145 */     this.crossFadingEnabled.setSelected(this.venue.isNavigationCrossFadingEnabled());
/* 146 */     this.crossFaderProperties.setCrossFader(this.venue.getNavigationCrossFader());
/*     */ 
/* 148 */     this.disableVolumeMonitoring.setSelected(!this.venue.isVolumeMonitoringEnabled());
/* 149 */     this.monitoringDelay.setText(String.valueOf(this.venue.getVolumeMonitoringDelay()));
/*     */   }
/*     */ 
/*     */   private boolean isFileNameExtension(File pFile, String pExtension)
/*     */   {
/* 155 */     String wFileName = pFile.getName();
/*     */ 
/* 157 */     int wEi = wFileName.lastIndexOf('.');
/*     */ 
/* 159 */     if (wEi != -1) {
/* 160 */       return wFileName.substring(wEi + 1).equalsIgnoreCase(pExtension);
/*     */     }
/*     */ 
/* 163 */     return false;
/*     */   }
/*     */ 
/*     */   private void btnCancel(JButton pButton)
/*     */   {
/* 168 */     dispose();
/*     */   }
/*     */ 
/*     */   private void btnOk(JButton pButton)
/*     */   {
/*     */     try
/*     */     {
/* 175 */       this.venue.setMicrophonePriorityGain(
/* 176 */         Float.parseFloat(this.microphonePriorityVolume.getText()));
/*     */ 
/* 178 */       this.venue.setNavigationCrossFadingEnabled(this.crossFadingEnabled.isSelected());
/* 179 */       this.venue.setNavigationCrossFader(this.crossFaderProperties.getCrossFader());
/*     */ 
/* 181 */       this.venue.setVolumeMonitoringEnabled(!this.disableVolumeMonitoring.isSelected());
/* 182 */       this.venue.setVolumeMonitoringDelay(Integer.parseInt(this.monitoringDelay.getText()));
/*     */     }
/*     */     catch (Exception localException)
/*     */     {
/*     */     }
/*     */ 
/* 189 */     dispose();
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJVenuePreferrences
 * JD-Core Version:    0.6.0
 */