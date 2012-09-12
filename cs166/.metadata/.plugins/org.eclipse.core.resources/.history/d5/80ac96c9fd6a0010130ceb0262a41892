/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Color;
/*     */ import java.awt.Component;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.Graphics;
/*     */ import java.awt.HeadlessException;
/*     */ import java.awt.Rectangle;
/*     */ import java.awt.event.MouseAdapter;
/*     */ import java.awt.event.MouseEvent;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JFrame;
/*     */ import kj.dsp.KJDigitalSignalProcessor;
/*     */ import kj.player.KJPlayer;
/*     */ import kj.ui.dsp.KJScopeAndSpectrumAnalyser;
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public class KJDSPDialog extends JDialog
/*     */   implements KJDigitalSignalProcessor, KJXmlSerializable
/*     */ {
/*  37 */   private boolean showDsp = true;
/*     */   private KJDigitalSignalProcessor dsp;
/*     */   private long sdlFramePosition;
/*     */   private long sdlFramePositionDelta;
/*     */   private int dspBufferPosition;
/*     */   private int dspAppendOffset;
/*     */   private int dspAppendLength;
/*     */   private int dspBufferSize;
/*     */   private long dspOutputPosition;
/*     */   private String dspInfo;
/*     */   private float averageVolume;
/*     */   private float maxAverageVolume;
/*     */ 
/*     */   public KJDSPDialog(JFrame pParent)
/*     */     throws HeadlessException
/*     */   {
/*  55 */     super(pParent);
/*     */ 
/*  57 */     setTitle("KJ - DSP");
/*     */ 
/*  59 */     setLayout(new BorderLayout());
/*     */ 
/*  61 */     this.dsp = new KJScopeAndSpectrumAnalyser();
/*     */ 
/*  63 */     add((Component)this.dsp);
/*     */ 
/*  65 */     prepareDisplayToggleListener((Component)this.dsp);
/*     */ 
/*  68 */     setSize(456, 208);
/*     */   }
/*     */ 
/*     */   private void drawDSPBufferInfo(Graphics pGrp)
/*     */   {
/*  74 */     Dimension wSize = getSize();
/*     */ 
/*  76 */     pGrp.setColor(Color.gray);
/*  77 */     pGrp.drawLine(0, (wSize.height >> 1) + 12, wSize.width, (wSize.height >> 1) + 12);
/*     */ 
/*  79 */     float wDtb = this.dspBufferSize / wSize.width;
/*     */ 
/*  82 */     int wOpp = (int)((float)this.dspOutputPosition / wDtb);
/*     */ 
/*  84 */     pGrp.setColor(Color.green);
/*  85 */     pGrp.drawLine(wOpp, 0, wOpp, wSize.height);
/*     */ 
/*  88 */     int wDbp = (int)(this.dspBufferPosition / wDtb);
/*     */ 
/*  90 */     pGrp.setColor(Color.red);
/*  91 */     pGrp.drawLine(wDbp, 0, wDbp, wSize.height);
/*     */ 
/*  93 */     pGrp.setColor(Color.white);
/*     */ 
/*  95 */     pGrp.drawString("SFP: " + this.sdlFramePosition + " [" + this.sdlFramePositionDelta + "]", 10, 48);
/*  96 */     pGrp.drawString("OBP: " + this.dspOutputPosition, 10, 60);
/*  97 */     pGrp.drawString("DBP: " + this.dspBufferPosition, 10, 72);
/*  98 */     pGrp.drawString("DAO: " + this.dspAppendOffset, 10, 86);
/*  99 */     pGrp.drawString("DAL: " + this.dspAppendLength, 10, 98);
/* 100 */     pGrp.drawString("DBS: " + this.dspBufferSize, 10, 110);
/* 101 */     pGrp.drawString("INF: " + this.dspInfo, 10, 122);
/* 102 */     pGrp.drawString("V/A: " + this.maxAverageVolume + " ( " + (int)(this.averageVolume / this.maxAverageVolume * 100.0F) + "% ) / " + this.averageVolume, 10, 134);
/*     */   }
/*     */ 
/*     */   private void prepareDisplayToggleListener(Component pTarget)
/*     */   {
/* 111 */     pTarget.addMouseListener(
/* 112 */       new MouseAdapter() {
/*     */       public void mouseClicked(MouseEvent pEvent) {
/* 114 */         if (pEvent.getButton() == 3)
/* 115 */           KJDSPDialog.this.showDsp = (!KJDSPDialog.this.showDsp);
/*     */       }
/*     */     });
/*     */   }
/*     */ 
/*     */   public void process(float[] pLeft, float[] pRight, float pFrameRateRatioHint)
/*     */   {
/* 125 */     Graphics wGrp = getGraphics();
/*     */ 
/* 127 */     if (this.showDsp)
/*     */     {
/* 129 */       this.dsp.process(pLeft, pRight, pFrameRateRatioHint);
/*     */     }
/*     */     else
/*     */     {
/* 133 */       wGrp.setColor(Color.black);
/* 134 */       wGrp.fillRect(0, 0, getSize().width, getSize().height);
/*     */ 
/* 136 */       drawDSPBufferInfo(wGrp);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void setAverageVolume(float pAverageVolume, float pMaxAverageVolume)
/*     */   {
/* 148 */     this.averageVolume = pAverageVolume;
/* 149 */     this.maxAverageVolume = pMaxAverageVolume;
/*     */   }
/*     */ 
/*     */   public void setDSPBufferInfo(int pDSPBufferPosition, int pOffset, int pLength, int pAdbl)
/*     */   {
/* 154 */     this.dspBufferPosition = pDSPBufferPosition;
/* 155 */     this.dspAppendOffset = pOffset;
/* 156 */     this.dspAppendLength = pLength;
/* 157 */     this.dspBufferSize = pAdbl;
/*     */   }
/*     */ 
/*     */   public void setDSPInformation(String pInfo)
/*     */   {
/* 162 */     this.dspInfo = pInfo;
/*     */   }
/*     */ 
/*     */   public void setOutputPositionInfo(long pSdlFramePosition, long pSdlFramePositionDelta, long pDspOutputPosition)
/*     */   {
/* 167 */     this.sdlFramePosition = pSdlFramePosition;
/* 168 */     this.sdlFramePositionDelta = pSdlFramePositionDelta;
/* 169 */     this.dspOutputPosition = pDspOutputPosition;
/*     */   }
/*     */ 
/*     */   public void setVisible(boolean pState)
/*     */   {
/* 175 */     super.setVisible(pState);
/*     */ 
/* 177 */     if (pState)
/* 178 */       KJJukeBox.getPlayer().addDigitalSignalProcessor(this);
/*     */     else
/* 180 */       KJJukeBox.getPlayer().removeDigitalSignalProcessor(this);
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource)
/*     */   {
/* 187 */     Element wBounds = KJXmlToolkit.find(pSource, "Bounds");
/*     */ 
/* 189 */     if (wBounds != null)
/*     */     {
/* 191 */       setBounds(
/* 192 */         Integer.parseInt(wBounds.getAttribute("x")), 
/* 193 */         Integer.parseInt(wBounds.getAttribute("y")), 
/* 194 */         Integer.parseInt(wBounds.getAttribute("width")), 
/* 195 */         Integer.parseInt(wBounds.getAttribute("height")));
/*     */     }
/*     */ 
/* 199 */     setVisible(KJXmlToolkit.get(pSource, "Visible", true));
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 208 */     Element wBounds = KJXmlToolkit.add(pTarget, "Bounds");
/*     */ 
/* 210 */     wBounds.setAttribute("x", String.valueOf(getBounds().x));
/* 211 */     wBounds.setAttribute("y", String.valueOf(getBounds().y));
/* 212 */     wBounds.setAttribute("width", String.valueOf(getBounds().width));
/* 213 */     wBounds.setAttribute("height", String.valueOf(getBounds().height));
/*     */ 
/* 215 */     KJXmlToolkit.set(pTarget, "Visible", isVisible());
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJDSPDialog
 * JD-Core Version:    0.6.0
 */