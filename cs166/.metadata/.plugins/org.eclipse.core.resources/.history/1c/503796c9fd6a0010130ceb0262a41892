/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.Dimension;
/*     */ import java.text.SimpleDateFormat;
/*     */ import java.util.Date;
/*     */ import javax.swing.ButtonGroup;
/*     */ import javax.swing.JCheckBox;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JRadioButton;
/*     */ import javax.swing.JTextField;
/*     */ import kj.track.KJTrackCrossFade;
/*     */ 
/*     */ public class KJCrossFaderProperties extends JPanel
/*     */ {
/*  33 */   private static final SimpleDateFormat SDF = new SimpleDateFormat("m:ss");
/*     */   private JTextField autoPosition;
/*     */   private JTextField autoVolumeThreshold;
/*     */   private JTextField autoPositionThreshold;
/*     */   private JTextField duration;
/*     */   private JRadioButton positionTypeAutomatic;
/*     */   private JRadioButton positionTypeEndOfTrack;
/*     */   private JRadioButton positionTypeDefault;
/*     */   private JTextField position;
/*     */   private JRadioButton volumeTypeNoFade;
/*     */   private JRadioButton volumeTypeDefault;
/*     */   private JTextField volumeStart;
/*     */   private JTextField volumeEnd;
/*     */   private JCheckBox startNextTrackAtEnabled;
/*     */   private JTextField startNextTrackAt;
/*     */   private boolean forNavigation;
/*     */ 
/*     */   public KJCrossFaderProperties()
/*     */   {
/*  57 */     this(false);
/*     */   }
/*     */ 
/*     */   public KJCrossFaderProperties(boolean pForNavigation)
/*     */   {
/*  62 */     super(new KJFlowLayout());
/*     */ 
/*  64 */     this.forNavigation = pForNavigation;
/*     */ 
/*  66 */     setBorder(KJUITools.createTitleBorder("Cross Fader Properties"));
/*     */ 
/*  69 */     if (!this.forNavigation)
/*     */     {
/*  71 */       add(new JLabel("<html><b>Position:</b></html>"));
/*     */ 
/*  73 */       ButtonGroup wPositionRadioButtonGroup = new ButtonGroup();
/*     */ 
/*  75 */       JPanel wAutoPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_VERTICAL));
/*     */ 
/*  77 */       add(wAutoPanel, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/*  79 */       this.positionTypeAutomatic = new JRadioButton("Automatic.");
/*     */ 
/*  81 */       wPositionRadioButtonGroup.add(this.positionTypeAutomatic);
/*  82 */       wAutoPanel.add(this.positionTypeAutomatic);
/*     */ 
/*  84 */       JPanel wAutoPositionPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/*  86 */       wAutoPanel.add(wAutoPositionPanel, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/*  88 */       this.autoPosition = new JTextField();
/*  89 */       this.autoPosition.setPreferredSize(new Dimension(48, 17));
/*  90 */       this.autoPosition.setHorizontalAlignment(4);
/*     */ 
/*  92 */       wAutoPositionPanel.add(new JLabel("- Monitor volume "));
/*  93 */       wAutoPositionPanel.add(this.autoPosition);
/*  94 */       wAutoPositionPanel.add(new JLabel(" millisecond before end of track."));
/*     */ 
/*  97 */       JPanel wAutoVolumeThresholdPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/*  99 */       wAutoPanel.add(wAutoVolumeThresholdPanel);
/*     */ 
/* 101 */       this.autoVolumeThreshold = new JTextField();
/* 102 */       this.autoVolumeThreshold.setPreferredSize(new Dimension(32, 17));
/* 103 */       this.autoVolumeThreshold.setHorizontalAlignment(4);
/*     */ 
/* 105 */       wAutoVolumeThresholdPanel.add(new JLabel("- Cross-fade when volume is "));
/* 106 */       wAutoVolumeThresholdPanel.add(this.autoVolumeThreshold);
/* 107 */       wAutoVolumeThresholdPanel.add(new JLabel("% of maximum track volume."));
/*     */ 
/* 110 */       JPanel wAutoPositionThresholdPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 112 */       wAutoPanel.add(wAutoPositionThresholdPanel);
/*     */ 
/* 114 */       this.autoPositionThreshold = new JTextField();
/* 115 */       this.autoPositionThreshold.setPreferredSize(new Dimension(48, 17));
/* 116 */       this.autoPositionThreshold.setHorizontalAlignment(4);
/*     */ 
/* 118 */       wAutoPositionThresholdPanel.add(new JLabel("- Force cross-fade "));
/* 119 */       wAutoPositionThresholdPanel.add(this.autoPositionThreshold);
/* 120 */       wAutoPositionThresholdPanel.add(new JLabel(" millisecond before end of track."));
/*     */ 
/* 125 */       this.positionTypeEndOfTrack = new JRadioButton("Before end of track.");
/*     */ 
/* 127 */       wPositionRadioButtonGroup.add(this.positionTypeEndOfTrack);
/* 128 */       add(this.positionTypeEndOfTrack);
/*     */ 
/* 130 */       this.positionTypeEndOfTrack.setSelected(true);
/*     */ 
/* 132 */       JPanel wPositionPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 134 */       add(wPositionPanel);
/*     */ 
/* 136 */       this.positionTypeDefault = new JRadioButton();
/*     */ 
/* 138 */       wPositionRadioButtonGroup.add(this.positionTypeDefault);
/* 139 */       wPositionPanel.add(this.positionTypeDefault);
/*     */ 
/* 141 */       JPanel wPositionRangePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 143 */       wPositionPanel.add(wPositionRangePanel);
/*     */ 
/* 145 */       this.position = new JTextField();
/* 146 */       this.position.setPreferredSize(new Dimension(48, 17));
/* 147 */       this.position.setHorizontalAlignment(4);
/*     */ 
/* 149 */       wPositionRangePanel.add(this.position);
/*     */ 
/* 151 */       wPositionRangePanel.add(new JLabel(" (m:ss)"));
/*     */ 
/* 153 */       KJUITools.setEnabled(wPositionRangePanel, false);
/*     */     }
/*     */ 
/* 158 */     JPanel wDurationPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_VERTICAL));
/*     */ 
/* 160 */     add(wDurationPanel, KJFlowLayout.CONSTRAINT_NO_INDENT);
/*     */ 
/* 162 */     wDurationPanel.add(new JLabel("<html><b>Duration:</b></html>"));
/*     */ 
/* 164 */     this.duration = new JTextField("10000");
/* 165 */     this.duration.setPreferredSize(new Dimension(48, 17));
/* 166 */     this.duration.setHorizontalAlignment(4);
/*     */ 
/* 168 */     JPanel wDurationValuePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 170 */     wDurationPanel.add(wDurationValuePanel, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/* 172 */     wDurationValuePanel.add(this.duration);
/* 173 */     wDurationValuePanel.add(new JLabel(" milliseconds."));
/*     */ 
/* 176 */     add(new JLabel("<html><b>Volume:</b></html>"));
/*     */ 
/* 178 */     ButtonGroup wVolumeRadioButtonGroup = new ButtonGroup();
/*     */ 
/* 180 */     if (!this.forNavigation)
/*     */     {
/* 182 */       this.volumeTypeNoFade = new JRadioButton("Do not fade volume.");
/*     */ 
/* 184 */       wVolumeRadioButtonGroup.add(this.volumeTypeNoFade);
/* 185 */       add(this.volumeTypeNoFade, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/* 187 */       this.volumeTypeNoFade.setSelected(true);
/*     */     }
/*     */ 
/* 191 */     JPanel wVolumePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 193 */     add(
/* 194 */       wVolumePanel, 
/* 195 */       !this.forNavigation ? KJFlowLayout.CONSTRAINT_NONE : KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/* 197 */     if (!this.forNavigation)
/*     */     {
/* 199 */       this.volumeTypeDefault = new JRadioButton();
/* 200 */       wVolumeRadioButtonGroup.add(this.volumeTypeDefault);
/*     */ 
/* 202 */       wVolumePanel.add(this.volumeTypeDefault);
/*     */     }
/*     */ 
/* 206 */     JPanel wVolumeRangePanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 208 */     wVolumePanel.add(wVolumeRangePanel);
/*     */ 
/* 210 */     wVolumeRangePanel.add(new JLabel("Start:"));
/*     */ 
/* 212 */     this.volumeStart = new JTextField("100.0");
/*     */ 
/* 214 */     wVolumeRangePanel.add(this.volumeStart);
/*     */ 
/* 216 */     this.volumeStart.setPreferredSize(new Dimension(48, 17));
/* 217 */     this.volumeStart.setHorizontalAlignment(4);
/*     */ 
/* 219 */     wVolumeRangePanel.add(new JLabel("% End:"));
/*     */ 
/* 221 */     this.volumeEnd = new JTextField("0.0");
/*     */ 
/* 223 */     wVolumeRangePanel.add(this.volumeEnd);
/*     */ 
/* 225 */     this.volumeEnd.setPreferredSize(new Dimension(48, 17));
/* 226 */     this.volumeEnd.setHorizontalAlignment(4);
/*     */ 
/* 228 */     wVolumeRangePanel.add(new JLabel("% "));
/*     */ 
/* 233 */     JPanel wStartNextTrackAtPanel = new JPanel(new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/* 235 */     add(wStartNextTrackAtPanel, KJFlowLayout.CONSTRAINT_INDENT);
/*     */ 
/* 237 */     this.startNextTrackAtEnabled = new JCheckBox("Start next track ");
/*     */ 
/* 239 */     wStartNextTrackAtPanel.add(this.startNextTrackAtEnabled);
/*     */ 
/* 241 */     this.startNextTrackAtEnabled.setPreferredSize(
/* 242 */       new Dimension(
/* 243 */       this.startNextTrackAtEnabled.getPreferredSize().width, 
/* 244 */       17));
/*     */ 
/* 246 */     this.startNextTrackAt = new JTextField("0");
/*     */ 
/* 248 */     wStartNextTrackAtPanel.add(this.startNextTrackAt);
/*     */ 
/* 250 */     this.startNextTrackAt.setPreferredSize(new Dimension(48, 17));
/* 251 */     this.startNextTrackAt.setHorizontalAlignment(4);
/*     */ 
/* 253 */     wStartNextTrackAtPanel.add(new JLabel(" millisecond into cross-fade."));
/*     */   }
/*     */ 
/*     */   public KJTrackCrossFade getCrossFader()
/*     */   {
/*     */     try
/*     */     {
/* 266 */       long wDuration = Long.parseLong(this.duration.getText());
/*     */ 
/* 268 */       long wPosition = 0L;
/* 269 */       float wAvt = 0.0F;
/* 270 */       long wAfnt = 0L;
/*     */ 
/* 272 */       if (!this.forNavigation)
/*     */       {
/* 274 */         if (this.positionTypeAutomatic.isSelected()) {
/* 275 */           wPosition = -Integer.parseInt(this.autoPosition.getText());
/* 276 */           wAvt = Float.parseFloat(this.autoVolumeThreshold.getText());
/* 277 */           wAfnt = Long.parseLong(this.autoPositionThreshold.getText());
/* 278 */         } else if (this.positionTypeDefault.isSelected()) {
/* 279 */           wPosition = SDF.parse(this.position.getText()).getTime() - 18000000L;
/*     */         } else {
/* 281 */           wPosition = -wDuration;
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/* 286 */       float wSv = 0.0F;
/* 287 */       float wEv = 0.0F;
/*     */ 
/* 289 */       if ((this.forNavigation) || (this.volumeTypeDefault.isSelected()))
/*     */       {
/* 291 */         wSv = Float.parseFloat(this.volumeStart.getText());
/* 292 */         wEv = Float.parseFloat(this.volumeEnd.getText());
/*     */       }
/*     */ 
/* 296 */       long wSnta = 0L;
/*     */ 
/* 298 */       if (this.startNextTrackAtEnabled.isSelected()) {
/* 299 */         wSnta = Integer.parseInt(this.startNextTrackAt.getText());
/*     */       }
/*     */ 
/* 304 */       return new KJTrackCrossFade(
/* 305 */         (this.positionTypeAutomatic != null) && (this.positionTypeAutomatic.isSelected()), 
/* 306 */         wPosition, 
/* 307 */         wDuration, 
/* 308 */         wSv, 
/* 309 */         wEv, 
/* 310 */         wSnta, 
/* 311 */         wAvt, 
/* 312 */         wAfnt);
/*     */     } catch (Exception pEx) {
/*     */     }
/* 315 */     return null;
/*     */   }
/*     */ 
/*     */   public void setCrossFader(KJTrackCrossFade pCrossFader)
/*     */   {
/* 322 */     if (pCrossFader == null) {
/* 323 */       return;
/*     */     }
/*     */ 
/* 326 */     if (!this.forNavigation)
/*     */     {
/* 328 */       if (pCrossFader.automatic)
/*     */       {
/* 330 */         this.positionTypeAutomatic.setSelected(true);
/* 331 */         this.autoPosition.setText(String.valueOf(-pCrossFader.position));
/* 332 */         this.autoVolumeThreshold.setText(String.valueOf(pCrossFader.automaticVolumeThreshold));
/* 333 */         this.autoPositionThreshold.setText(String.valueOf(pCrossFader.automaticPositionThreshold));
/*     */       }
/* 335 */       else if (pCrossFader.position >= 0L)
/*     */       {
/* 337 */         this.positionTypeDefault.setSelected(true);
/* 338 */         this.position.setText(SDF.format(new Date(pCrossFader.position)));
/*     */       }
/*     */       else {
/* 341 */         this.positionTypeEndOfTrack.setSelected(true);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 346 */     this.duration.setText(String.valueOf(pCrossFader.duration));
/*     */ 
/* 348 */     if ((pCrossFader.volumeAtStart == pCrossFader.volumeAtEnd) && (!this.forNavigation)) {
/* 349 */       this.volumeTypeNoFade.setSelected(true);
/*     */     }
/*     */     else {
/* 352 */       if (!this.forNavigation) {
/* 353 */         this.volumeTypeDefault.setSelected(true);
/*     */       }
/*     */ 
/* 356 */       this.volumeStart.setText(String.valueOf(pCrossFader.volumeAtStart));
/* 357 */       this.volumeEnd.setText(String.valueOf(pCrossFader.volumeAtEnd));
/*     */     }
/*     */ 
/* 361 */     if (pCrossFader.startNextTrackAt > 0L) {
/* 362 */       this.startNextTrackAtEnabled.setSelected(true);
/* 363 */       this.startNextTrackAt.setText(String.valueOf(pCrossFader.startNextTrackAt));
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJCrossFaderProperties
 * JD-Core Version:    0.6.0
 */