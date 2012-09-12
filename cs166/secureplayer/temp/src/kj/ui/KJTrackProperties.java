/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.HeadlessException;
/*     */ import java.awt.event.ItemEvent;
/*     */ import java.awt.event.ItemListener;
/*     */ import java.io.File;
/*     */ import java.util.Hashtable;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JCheckBox;
/*     */ import javax.swing.JComboBox;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JFrame;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JSlider;
/*     */ import javax.swing.JTextField;
/*     */ import javax.swing.event.ChangeEvent;
/*     */ import javax.swing.event.ChangeListener;
/*     */ import kj.player.KJPlayer;
/*     */ import kj.track.KJTrack;
/*     */ import kj.track.KJTrackCrossFade;
/*     */ 
/*     */ public class KJTrackProperties extends JDialog
/*     */ {
/*     */   private static final String CROSS_FADE_MODE_DISABLED = "Disabled";
/*     */   private static final String CROSS_FADE_MODE_PLAY_LIST = "Use Play List Cross-Fader";
/*     */   private static final String CROSS_FADE_MODE_CUSTOM = "Custom";
/*     */   private KJTrack[] tracks;
/*     */   private JTextField gain;
/*     */   private JSlider gainSlider;
/*     */   private JComboBox crossFadingMode;
/*     */   private JCheckBox allowRequestFollowingTrack;
/*     */   private KJCrossFaderProperties crossFaderProperties;
/*     */   private float oldGain;
/*     */ 
/*     */   public KJTrackProperties(JFrame pParent, KJTrack[] pTracks)
/*     */     throws HeadlessException
/*     */   {
/*  58 */     super(pParent, true);
/*     */ 
/*  60 */     this.tracks = pTracks;
/*     */ 
/*  62 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   public KJTrackProperties(JDialog pParent, KJTrack[] pTracks)
/*     */     throws HeadlessException
/*     */   {
/*  68 */     super(pParent, true);
/*     */ 
/*  70 */     this.tracks = pTracks;
/*     */ 
/*  72 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/*  78 */     String wTitle = "KJ - Track Properties";
/*     */ 
/*  80 */     if (this.tracks.length == 1)
/*  81 */       wTitle = wTitle + " [" + this.tracks[0].toString() + "]";
/*     */     else {
/*  83 */       wTitle = wTitle + " [Multiple Tracks]";
/*     */     }
/*     */ 
/*  86 */     setTitle(wTitle);
/*  87 */     setSize(600, 512);
/*     */ 
/*  89 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/*  91 */     JPanel wOptions = new JPanel(new KJFlowLayout());
/*     */ 
/*  93 */     getContentPane().add(wOptions, "Center");
/*     */ 
/*  95 */     wOptions.setBorder(KJUITools.createTitleBorder("Track Properties"));
/*     */ 
/*  98 */     JPanel wGainOptionsPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_VERTICAL));
/*     */ 
/* 100 */     wOptions.add(wGainOptionsPanel);
/*     */ 
/* 102 */     wGainOptionsPanel.setBorder(KJUITools.createTitleBorder("Gain"));
/*     */ 
/* 104 */     JPanel wGainValuePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 106 */     wGainValuePanel.add(new JLabel("Track Gain: "));
/*     */ 
/* 108 */     this.gain = new JTextField();
/*     */ 
/* 110 */     this.gain.setPreferredSize(new Dimension(48, 17));
/* 111 */     this.gain.setHorizontalAlignment(4);
/* 112 */     this.gain.setEditable(false);
/*     */ 
/* 114 */     wGainValuePanel.add(this.gain);
/*     */ 
/* 116 */     wGainValuePanel.add(new JLabel("%  ( -100% to 100% )"));
/*     */ 
/* 118 */     wGainOptionsPanel.add(wGainValuePanel);
/*     */ 
/* 120 */     JPanel wGainSliderPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 124 */     int wMgp = (int)KJJukeBox.getPlayer().getMaximumGainPercentage() - 100;
/*     */ 
/* 126 */     this.gainSlider = new JSlider(0, -100, wMgp, 0);
/*     */ 
/* 128 */     Hashtable wGainSliderLabels = new Hashtable();
/*     */ 
/* 130 */     wGainSliderLabels.put(new Integer(-100), new JLabel("-100%"));
/* 131 */     wGainSliderLabels.put(new Integer(-50), new JLabel("-50%"));
/* 132 */     wGainSliderLabels.put(new Integer(0), new JLabel("0"));
/* 133 */     wGainSliderLabels.put(new Integer(50), new JLabel("50%"));
/* 134 */     wGainSliderLabels.put(new Integer(wMgp), new JLabel(String.valueOf(wMgp) + "%"));
/*     */ 
/* 136 */     this.gainSlider.setLabelTable(wGainSliderLabels);
/* 137 */     this.gainSlider.setPaintLabels(true);
/*     */ 
/* 139 */     this.gainSlider.setMinorTickSpacing(10);
/* 140 */     this.gainSlider.setMajorTickSpacing(100);
/* 141 */     this.gainSlider.setPaintTicks(true);
/*     */ 
/* 143 */     this.gainSlider.setPreferredSize(new Dimension(340, 48));
/*     */ 
/* 145 */     this.gainSlider.addChangeListener(
/* 146 */       new ChangeListener() {
/*     */       public void stateChanged(ChangeEvent pEvent) {
/* 148 */         KJTrackProperties.this.gain.setText(String.valueOf(KJTrackProperties.this.gainSlider.getValue()));
/* 149 */         KJTrackProperties.this.tracks[0].setGain(KJTrackProperties.this.gainSlider.getValue());
/*     */       }
/*     */     });
/* 154 */     wGainSliderPanel.add(this.gainSlider);
/*     */ 
/* 156 */     wGainOptionsPanel.add(wGainSliderPanel);
/*     */ 
/* 159 */     JPanel wRequestOptions = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 161 */     wOptions.add(wRequestOptions);
/*     */ 
/* 163 */     wRequestOptions.setBorder(KJUITools.createTitleBorder("Request Options"));
/*     */ 
/* 165 */     this.allowRequestFollowingTrack = new JCheckBox("Allow requests to follow track(s)");
/*     */ 
/* 167 */     wRequestOptions.add(this.allowRequestFollowingTrack);
/*     */ 
/* 170 */     JPanel wCrossFadingPanel = new JPanel(new BorderLayout());
/*     */ 
/* 172 */     wOptions.add(wCrossFadingPanel);
/*     */ 
/* 174 */     wCrossFadingPanel.setBorder(KJUITools.createTitleBorder("Cross Fader"));
/*     */ 
/* 176 */     this.crossFadingMode = 
/* 180 */       new JComboBox(new String[] { 
/* 178 */       "Disabled", 
/* 179 */       "Use Play List Cross-Fader", 
/* 180 */       "Custom" });
/*     */ 
/* 184 */     this.crossFadingMode.addItemListener(
/* 185 */       new ItemListener() {
/*     */       public void itemStateChanged(ItemEvent pEvent) {
/* 187 */         KJUITools.setEnabled(
/* 188 */           KJTrackProperties.this.crossFaderProperties, 
/* 189 */           pEvent.getItem() == "Custom");
/*     */       }
/*     */     });
/* 194 */     wCrossFadingPanel.add(this.crossFadingMode, "North");
/*     */ 
/* 196 */     this.crossFaderProperties = new KJCrossFaderProperties();
/*     */ 
/* 198 */     wCrossFadingPanel.add(this.crossFaderProperties, "Center");
/*     */ 
/* 200 */     KJUITools.setEnabled(this.crossFaderProperties, false);
/*     */ 
/* 203 */     JPanel wButtons = new JPanel();
/*     */ 
/* 205 */     getContentPane().add(wButtons, "South");
/*     */ 
/* 207 */     wButtons.add(KJUITools.createButton("Ok", this));
/* 208 */     wButtons.add(KJUITools.createButton("Cancel", this));
/*     */ 
/* 210 */     initializeValues();
/*     */ 
/* 212 */     pack();
/*     */ 
/* 214 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*     */ 
/* 216 */     setVisible(true);
/*     */   }
/*     */ 
/*     */   private void initializeValues()
/*     */   {
/* 222 */     KJTrack wBaseTrack = this.tracks[0];
/*     */ 
/* 224 */     this.oldGain = wBaseTrack.getGain();
/*     */ 
/* 226 */     this.gain.setText(String.valueOf(wBaseTrack.getGain()));
/* 227 */     this.gainSlider.setValue((int)wBaseTrack.getGain());
/*     */ 
/* 229 */     this.allowRequestFollowingTrack.setSelected(wBaseTrack.getAllowRequestsToFollow());
/*     */ 
/* 231 */     if (wBaseTrack.isCrossFadingEnabled())
/*     */     {
/* 233 */       if (wBaseTrack.getCrossFade() == null) {
/* 234 */         this.crossFadingMode.setSelectedItem("Use Play List Cross-Fader");
/*     */       } else {
/* 236 */         this.crossFadingMode.setSelectedItem("Custom");
/* 237 */         this.crossFaderProperties.setCrossFader(wBaseTrack.getCrossFade());
/*     */       }
/*     */     }
/*     */     else
/* 241 */       this.crossFadingMode.setSelectedItem("Disabled");
/*     */   }
/*     */ 
/*     */   private boolean isFileNameExtension(File pFile, String pExtension)
/*     */   {
/* 248 */     String wFileName = pFile.getName();
/*     */ 
/* 250 */     int wEi = wFileName.lastIndexOf('.');
/*     */ 
/* 252 */     if (wEi != -1) {
/* 253 */       return wFileName.substring(wEi + 1).equalsIgnoreCase(pExtension);
/*     */     }
/*     */ 
/* 256 */     return false;
/*     */   }
/*     */ 
/*     */   private void btnCancel(JButton pButton)
/*     */   {
/* 263 */     this.tracks[0].setGain(this.oldGain);
/*     */ 
/* 265 */     dispose();
/*     */   }
/*     */ 
/*     */   private void btnOk(JButton pButton)
/*     */   {
/* 271 */     float wGain = 0.0F;
/*     */     try
/*     */     {
/* 274 */       wGain = Float.parseFloat(this.gain.getText());
/*     */     }
/*     */     catch (Exception localException)
/*     */     {
/*     */     }
/* 279 */     boolean wArtf = this.allowRequestFollowingTrack.isSelected();
/*     */ 
/* 281 */     String wCfm = (String)this.crossFadingMode.getSelectedItem();
/*     */ 
/* 283 */     boolean wCfe = true;
/* 284 */     KJTrackCrossFade wTcf = null;
/*     */ 
/* 286 */     if (wCfm == "Custom")
/* 287 */       wTcf = this.crossFaderProperties.getCrossFader();
/* 288 */     else if (wCfm == "Disabled") {
/* 289 */       wCfe = false;
/*     */     }
/*     */ 
/* 292 */     for (int a = 0; a < this.tracks.length; a++)
/*     */     {
/* 294 */       KJTrack wTrack = this.tracks[a];
/*     */ 
/* 296 */       wTrack.setCrossFadingEnabled(wCfe);
/* 297 */       wTrack.setCrossFade(wTcf);
/*     */ 
/* 299 */       wTrack.setAllowRequestsToFollow(wArtf);
/*     */ 
/* 301 */       wTrack.setGain(wGain);
/*     */     }
/*     */ 
/* 305 */     dispose();
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJTrackProperties
 * JD-Core Version:    0.6.0
 */