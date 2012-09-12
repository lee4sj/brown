/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.Component;
/*     */ import java.awt.Container;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.Insets;
/*     */ import java.awt.LayoutManager2;
/*     */ import java.awt.Rectangle;
/*     */ import java.util.ArrayList;
/*     */ 
/*     */ public class KJVerticalFlowLayout
/*     */   implements LayoutManager2
/*     */ {
/*  24 */   public static Integer CONSTRAINTS_SEPARATOR = new Integer(1);
/*     */ 
/*  26 */   public static int ALIGNMENT_TOP = 1;
/*  27 */   public static int ALIGNMENT_CENTER = 2;
/*  28 */   public static int ALIGNMENT_BOTTOM = 3;
/*     */ 
/*  30 */   public static int DEFAULT_ALIGNMENT = ALIGNMENT_TOP;
/*  31 */   public static int DEFAULT_COMPONENT_GAP_SIZE = 1;
/*  32 */   public static int DEFAULT_SEPARATOR_SIZE = 12;
/*     */ 
/*  34 */   private ArrayList constraints = new ArrayList();
/*     */   private int alignment;
/*     */   private int gapSize;
/*     */   private int separatorSize;
/*     */   private Dimension preferredSize;
/*     */ 
/*     */   public KJVerticalFlowLayout()
/*     */   {
/*  46 */     this(DEFAULT_ALIGNMENT);
/*     */   }
/*     */ 
/*     */   public KJVerticalFlowLayout(int pAlignment) {
/*  50 */     this(pAlignment, DEFAULT_COMPONENT_GAP_SIZE, DEFAULT_SEPARATOR_SIZE);
/*     */   }
/*     */ 
/*     */   public KJVerticalFlowLayout(int pGapSize, int pSeparatorSize) {
/*  54 */     this(DEFAULT_ALIGNMENT, pGapSize, pSeparatorSize);
/*     */   }
/*     */ 
/*     */   public KJVerticalFlowLayout(int pAlignment, int pGapSize, int pSeparatorSize) {
/*  58 */     this.alignment = pAlignment;
/*  59 */     this.gapSize = pGapSize;
/*  60 */     this.separatorSize = pSeparatorSize;
/*     */   }
/*     */ 
/*     */   public float getLayoutAlignmentX(Container pContainer)
/*     */   {
/*  67 */     return 0.0F;
/*     */   }
/*     */ 
/*     */   public float getLayoutAlignmentY(Container pContainer)
/*     */   {
/*  74 */     return 0.0F;
/*     */   }
/*     */ 
/*     */   public void invalidateLayout(Container pContainer)
/*     */   {
/*  82 */     Insets wInsets = pContainer.getInsets();
/*     */ 
/*  84 */     int y = wInsets.top;
/*  85 */     int wMw = 0;
/*     */ 
/*  87 */     for (int a = 0; a < this.constraints.size(); a++)
/*     */     {
/*  89 */       Constraint wConstraint = (Constraint)this.constraints.get(a);
/*     */ 
/*  91 */       if (wConstraint.separator) {
/*  92 */         y += this.separatorSize;
/*     */       }
/*     */ 
/*  95 */       Component wComponent = wConstraint.component;
/*     */ 
/*  97 */       Dimension wPs = wComponent.getPreferredSize();
/*     */ 
/*  99 */       wConstraint.bounds = new Rectangle(wInsets.left, y, wPs.width, wPs.height);
/*     */ 
/* 101 */       if (wPs.width > wMw) {
/* 102 */         wMw = wPs.width;
/*     */       }
/*     */ 
/* 105 */       y += wPs.height + (!wConstraint.separator ? this.gapSize : 0);
/*     */     }
/*     */ 
/* 110 */     for (int a = 0; a < this.constraints.size(); a++)
/* 111 */       ((Constraint)this.constraints.get(a)).bounds.width = wMw;
/*     */   }
/*     */ 
/*     */   public Dimension maximumLayoutSize(Container pContainer)
/*     */   {
/* 120 */     return new Dimension(2147483647, 2147483647);
/*     */   }
/*     */ 
/*     */   public void addLayoutComponent(Component pComponent, Object pConstraints)
/*     */   {
/* 127 */     this.constraints.add(new Constraint(pComponent, pConstraints == CONSTRAINTS_SEPARATOR));
/*     */   }
/*     */ 
/*     */   public void removeLayoutComponent(Component pComponent)
/*     */   {
/* 135 */     for (int a = 0; a < this.constraints.size(); a++)
/*     */     {
/* 137 */       Constraint wConstraint = (Constraint)this.constraints.get(a);
/*     */ 
/* 139 */       if (wConstraint.component == pComponent) {
/* 140 */         this.constraints.remove(a);
/* 141 */         return;
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   public void layoutContainer(Container pContainer)
/*     */   {
/* 153 */     for (int a = 0; a < this.constraints.size(); a++)
/*     */     {
/* 155 */       Constraint wConstraint = (Constraint)this.constraints.get(a);
/*     */ 
/* 157 */       wConstraint.component.setBounds(wConstraint.bounds);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void addLayoutComponent(String arg0, Component arg1)
/*     */   {
/*     */   }
/*     */ 
/*     */   public Dimension minimumLayoutSize(Container pContainer)
/*     */   {
/* 174 */     return getPreferredLayoutSize(pContainer);
/*     */   }
/*     */ 
/*     */   public Dimension preferredLayoutSize(Container pContainer)
/*     */   {
/* 181 */     return getPreferredLayoutSize(pContainer);
/*     */   }
/*     */ 
/*     */   private Dimension getPreferredLayoutSize(Container pContainer)
/*     */   {
/* 186 */     Insets wInsets = pContainer.getInsets();
/*     */ 
/* 188 */     Rectangle wBounds = ((Constraint)this.constraints.get(this.constraints.size() - 1)).bounds;
/*     */ 
/* 190 */     Dimension wPs = new Dimension(
/* 191 */       wBounds.x + wBounds.width + wInsets.right, 
/* 192 */       wBounds.y + wBounds.height + wInsets.bottom);
/*     */ 
/* 196 */     return wPs;
/*     */   }
/*     */ 
/*     */   private class Constraint {
/*     */     public Component component;
/*     */     public boolean separator;
/*     */     public Rectangle bounds;
/*     */ 
/*     */     public Constraint(Component pComponent, boolean pSeparator) {
/* 208 */       this.component = pComponent;
/* 209 */       this.separator = pSeparator;
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJVerticalFlowLayout
 * JD-Core Version:    0.6.0
 */