/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.Color;
/*     */ import java.awt.Component;
/*     */ import javax.swing.JLabel;
/*     */ import javax.swing.JTable;
/*     */ import javax.swing.table.DefaultTableCellRenderer;
/*     */ import kj.track.KJTrack;
/*     */ 
/*     */ public class KJPlayListEntryRenderer extends DefaultTableCellRenderer
/*     */ {
/*  28 */   private static final Color COLOR_GREEN = new Color(128, 255, 64);
/*     */ 
/*  32 */   private static final Color COLOR_PURPLE = new Color(224, 96, 255);
/*     */ 
/*  35 */   private static final Color COLOR_YELLOW = new Color(255, 255, 64);
/*     */ 
/*  38 */   private static final Color COLOR_BLUE = new Color(64, 128, 255);
/*     */ 
/*  40 */   private static final Color COLOR_ORANGE = new Color(255, 192, 64);
/*     */   private boolean renderStateColors;
/*     */ 
/*     */   public KJPlayListEntryRenderer(boolean pRenderStateColors)
/*     */   {
/*  45 */     this.renderStateColors = pRenderStateColors;
/*     */   }
/*     */ 
/*     */   public Component getTableCellRendererComponent(JTable pTable, Object pValue, boolean pIsSelected, boolean pHasFocus, int pRow, int pColumn)
/*     */   {
/*  51 */     KJTrack wPle = (KJTrack)pValue;
/*     */ 
/*  53 */     JLabel wTcr = (JLabel)super.getTableCellRendererComponent(
/*  54 */       pTable, 
/*  55 */       pRow + 1 + ". " + wPle.toString(), 
/*  56 */       pIsSelected, 
/*  57 */       pHasFocus, 
/*  58 */       pRow, 
/*  59 */       pColumn);
/*     */ 
/*  61 */     if (this.renderStateColors)
/*     */     {
/*  63 */       Color wColor = pTable.getBackground();
/*     */ 
/*  65 */       switch (wPle.getState())
/*     */       {
/*     */       case -1:
/*  68 */         wColor = Color.red;
/*  69 */         break;
/*     */       case 3:
/*  72 */         wColor = COLOR_YELLOW;
/*  73 */         break;
/*     */       case 1:
/*  76 */         wColor = COLOR_GREEN;
/*  77 */         break;
/*     */       case 2:
/*  80 */         wColor = Color.lightGray;
/*     */       case 0:
/*     */       default:
/*  85 */         if (KJJukeBox.getNextTrack() == wPle) {
/*  86 */           wColor = COLOR_ORANGE; } else {
/*  87 */           if (!wPle.isRequested()) break;
/*  88 */           wColor = COLOR_PURPLE;
/*     */         }
/*     */ 
/*     */       }
/*     */ 
/*  93 */       if (pIsSelected) {
/*  94 */         wTcr.setForeground(wColor);
/*  95 */         wTcr.setBackground(pTable.getSelectionBackground());
/*     */       } else {
/*  97 */         wTcr.setForeground(pTable.getForeground());
/*  98 */         wTcr.setBackground(wColor);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 103 */     return wTcr;
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJPlayListEntryRenderer
 * JD-Core Version:    0.6.0
 */