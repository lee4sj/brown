/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.HeadlessException;
/*     */ import javax.swing.ButtonGroup;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JComboBox;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JRadioButton;
/*     */ import javax.swing.JTextField;
/*     */ import kj.venue.KJVenue;
/*     */ 
/*     */ public class KJRequestListPreferrences extends JDialog
/*     */ {
/*     */   private static final String REQUEST_INTERVAL_MODE_MINUTES = "minutes";
/*     */   private static final String REQUEST_INTERVAL_MODE_TRACKS = "tracks";
/*     */   private KJVenue venue;
/*     */   private JRadioButton requestingDisabled;
/*     */   private JRadioButton requestingAtInterval;
/*     */   private JComboBox requestIntervalMode;
/*     */   private JTextField requestIntervalStart;
/*     */   private JTextField requestIntervalEnd;
/*     */   private JRadioButton requestingImmediately;
/*     */ 
/*     */   public KJRequestListPreferrences(JDialog pParent, KJVenue pVenue)
/*     */     throws HeadlessException
/*     */   {
/*  49 */     super(pParent, true);
/*     */ 
/*  51 */     this.venue = pVenue;
/*     */ 
/*  53 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/*  59 */     setTitle("KJ - Request List Preferences");
/*  60 */     setSize(512, 512);
/*     */ 
/*  62 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/*  64 */     JPanel wOptions = new JPanel(
/*  65 */       new KJFlowLayout(KJFlowLayout.ORIENTATION_VERTICAL));
/*     */ 
/*  67 */     wOptions.setBorder(KJUITools.createTitleBorder("Request Mode"));
/*     */ 
/*  69 */     getContentPane().add(wOptions, "Center");
/*     */ 
/*  71 */     ButtonGroup wButtonGroup = new ButtonGroup();
/*     */ 
/*  74 */     this.requestingDisabled = new JRadioButton("Do not play requests.");
/*     */ 
/*  76 */     wOptions.add(this.requestingDisabled, KJFlowLayout.CONSTRAINT_NONE);
/*  77 */     wButtonGroup.add(this.requestingDisabled);
/*     */ 
/*  80 */     JPanel wRequestIntervalOptions = new JPanel(
/*  81 */       new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*     */ 
/*  83 */     wOptions.add(wRequestIntervalOptions, KJFlowLayout.CONSTRAINT_NONE);
/*     */ 
/*  85 */     this.requestingAtInterval = new JRadioButton("Play a requested track every");
/*     */ 
/*  87 */     wRequestIntervalOptions.add(this.requestingAtInterval);
/*  88 */     wButtonGroup.add(this.requestingAtInterval);
/*     */ 
/*  90 */     this.requestIntervalStart = new JTextField();
/*  91 */     this.requestIntervalStart.setHorizontalAlignment(4);
/*  92 */     this.requestIntervalStart.setPreferredSize(new Dimension(32, 0));
/*     */ 
/*  94 */     wRequestIntervalOptions.add(this.requestIntervalStart);
/*     */ 
/*  96 */     wRequestIntervalOptions.add(new JLabel(" to "));
/*     */ 
/*  98 */     this.requestIntervalEnd = new JTextField();
/*  99 */     this.requestIntervalEnd.setHorizontalAlignment(4);
/* 100 */     this.requestIntervalEnd.setPreferredSize(new Dimension(32, 0));
/*     */ 
/* 102 */     wRequestIntervalOptions.add(this.requestIntervalEnd);
/*     */ 
/* 104 */     this.requestIntervalMode = 
/* 107 */       new JComboBox(new String[] { 
/* 106 */       "minutes", 
/* 107 */       "tracks" });
/*     */ 
/* 111 */     this.requestIntervalMode.setPreferredSize(new Dimension(64, 0));
/*     */ 
/* 113 */     wRequestIntervalOptions.add(this.requestIntervalMode);
/*     */ 
/* 116 */     this.requestingImmediately = new JRadioButton("Play requests immediately.");
/*     */ 
/* 118 */     wOptions.add(this.requestingImmediately, KJFlowLayout.CONSTRAINT_NONE);
/* 119 */     wButtonGroup.add(this.requestingImmediately);
/*     */ 
/* 122 */     JPanel wButtons = new JPanel();
/*     */ 
/* 124 */     getContentPane().add(wButtons, "South");
/*     */ 
/* 126 */     wButtons.add(KJUITools.createButton("Ok", this));
/* 127 */     wButtons.add(KJUITools.createButton("Cancel", this));
/*     */ 
/* 129 */     initializeValues();
/*     */ 
/* 131 */     pack();
/*     */ 
/* 133 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*     */ 
/* 135 */     setVisible(true);
/*     */   }
/*     */ 
/*     */   private void initializeValues()
/*     */   {
/* 141 */     switch (this.venue.getRequestIntervalMode())
/*     */     {
/*     */     case 0:
/* 144 */       this.requestingDisabled.setSelected(true);
/* 145 */       break;
/*     */     case 2:
/* 148 */       this.requestIntervalMode.setSelectedItem("tracks");
/*     */     case 1:
/* 152 */       this.requestingAtInterval.setSelected(true);
/* 153 */       break;
/*     */     case 3:
/* 156 */       this.requestingImmediately.setSelected(true);
/*     */     }
/*     */ 
/* 161 */     this.requestIntervalStart.setText(String.valueOf(this.venue.getRequestIntervalStart()));
/* 162 */     this.requestIntervalEnd.setText(String.valueOf(this.venue.getRequestIntervalEnd()));
/*     */   }
/*     */ 
/*     */   private void btnCancel(JButton pButton)
/*     */   {
/* 167 */     dispose();
/*     */   }
/*     */ 
/*     */   private void btnOk(JButton pButton)
/*     */   {
/*     */     try
/*     */     {
/* 174 */       int wRim = 0;
/*     */ 
/* 176 */       if (this.requestingAtInterval.isSelected())
/*     */       {
/* 178 */         if (this.requestIntervalMode.getSelectedItem() == "minutes")
/* 179 */           wRim = 1;
/*     */         else {
/* 181 */           wRim = 2;
/*     */         }
/*     */       }
/* 184 */       else if (this.requestingImmediately.isSelected()) {
/* 185 */         wRim = 3;
/*     */       }
/*     */ 
/* 188 */       this.venue.setRequestIntervalMode(wRim);
/* 189 */       this.venue.setRequestIntervalStart(Integer.parseInt(this.requestIntervalStart.getText()));
/* 190 */       this.venue.setRequestIntervalEnd(Integer.parseInt(this.requestIntervalEnd.getText()));
/*     */     }
/*     */     catch (Exception localException)
/*     */     {
/*     */     }
/*     */ 
/* 196 */     dispose();
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJRequestListPreferrences
 * JD-Core Version:    0.6.0
 */