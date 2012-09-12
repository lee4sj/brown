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
/*     */ public class KJFlowLayout
/*     */   implements LayoutManager2
/*     */ {
/*  24 */   public static Integer CONSTRAINT_NONE = new Integer(0);
/*  25 */   public static Integer CONSTRAINT_SEPARATOR = new Integer(1);
/*  26 */   public static Integer CONSTRAINT_INDENT = new Integer(2);
/*  27 */   public static Integer CONSTRAINT_UNDENT = new Integer(3);
/*  28 */   public static Integer CONSTRAINT_NO_INDENT = new Integer(4);
/*     */ 
/*  30 */   public static int ORIENTATION_HORIZONTAL = 1;
/*  31 */   public static int ORIENTATION_VERTICAL = 2;
/*     */ 
/*  33 */   public static int DEFAULT_ORIENTATION = ORIENTATION_VERTICAL;
/*  34 */   public static int DEFAULT_COMPONENT_GAP_SIZE = 2;
/*  35 */   public static int DEFAULT_SEPARATOR_SIZE = 12;
/*     */ 
/*  37 */   private ArrayList constraints = new ArrayList();
/*     */   private int gapSize;
/*     */   private int orientation;
/*     */   private int separatorSize;
/*     */   private Dimension preferredSize;
/*     */ 
/*     */   public KJFlowLayout()
/*     */   {
/*  46 */     this(DEFAULT_ORIENTATION);
/*     */   }
/*     */ 
/*     */   public KJFlowLayout(int pOrientation) {
/*  50 */     this(pOrientation, DEFAULT_COMPONENT_GAP_SIZE, DEFAULT_SEPARATOR_SIZE);
/*     */   }
/*     */ 
/*     */   public KJFlowLayout(int pGapSize, int pSeparatorSize) {
/*  54 */     this(DEFAULT_ORIENTATION, pGapSize, pSeparatorSize);
/*     */   }
/*     */ 
/*     */   public KJFlowLayout(int pOrientation, int pGapSize, int pSeparatorSize) {
/*  58 */     this.orientation = pOrientation;
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
/*  84 */     int x = wInsets.left;
/*  85 */     int y = wInsets.top;
/*  86 */     int wMx = 0;
/*     */ 
/*  88 */     for (int a = 0; a < this.constraints.size(); a++)
/*     */     {
/*  90 */       Constraint wConstraint = (Constraint)this.constraints.get(a);
/*     */ 
/*  92 */       if (this.orientation == ORIENTATION_VERTICAL)
/*     */       {
/*  94 */         if (wConstraint.type == CONSTRAINT_SEPARATOR)
/*  95 */           y += this.separatorSize;
/*  96 */         else if (wConstraint.type == CONSTRAINT_INDENT)
/*  97 */           x += this.separatorSize;
/*  98 */         else if (wConstraint.type == CONSTRAINT_UNDENT)
/*  99 */           x -= this.separatorSize;
/* 100 */         else if (wConstraint.type == CONSTRAINT_NO_INDENT) {
/* 101 */           x = wInsets.left;
/*     */         }
/*     */ 
/*     */       }
/* 106 */       else if (wConstraint.type == CONSTRAINT_SEPARATOR)
/* 107 */         x += this.separatorSize;
/* 108 */       else if (wConstraint.type == CONSTRAINT_INDENT)
/* 109 */         y += this.separatorSize;
/* 110 */       else if (wConstraint.type == CONSTRAINT_UNDENT)
/* 111 */         y -= this.separatorSize;
/* 112 */       else if (wConstraint.type == CONSTRAINT_NO_INDENT) {
/* 113 */         y = wInsets.top;
/*     */       }
/*     */ 
/* 118 */       Component wComponent = wConstraint.component;
/*     */ 
/* 120 */       Dimension wPs = wComponent.getPreferredSize();
/*     */ 
/* 122 */       wConstraint.bounds = new Rectangle(x, y, wPs.width, wPs.height);
/*     */ 
/* 124 */       if (this.orientation == ORIENTATION_VERTICAL)
/*     */       {
/* 126 */         if (x + wPs.width > wMx) {
/* 127 */           wMx = x + wPs.width;
/*     */         }
/*     */ 
/* 130 */         y += wPs.height + (wConstraint.type != CONSTRAINT_SEPARATOR ? this.gapSize : 0);
/*     */       }
/*     */       else
/*     */       {
/* 134 */         if (y + wPs.height > wMx) {
/* 135 */           wMx = y + wPs.height;
/*     */         }
/*     */ 
/* 138 */         x += wPs.width + (wConstraint.type != CONSTRAINT_SEPARATOR ? this.gapSize : 0);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 145 */     if (this.orientation == ORIENTATION_VERTICAL)
/*     */     {
/* 147 */       for (int a = 0; a < this.constraints.size(); a++)
/*     */       {
/* 149 */         Rectangle wBounds = ((Constraint)this.constraints.get(a)).bounds;
/*     */ 
/* 151 */         wBounds.width = (wMx - wBounds.x);
/*     */       }
/*     */ 
/*     */     }
/*     */     else
/*     */     {
/* 157 */       for (int a = 0; a < this.constraints.size(); a++)
/*     */       {
/* 159 */         Rectangle wBounds = ((Constraint)this.constraints.get(a)).bounds;
/*     */ 
/* 161 */         wBounds.height = (wMx - wBounds.y);
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 167 */     this.preferredSize = null;
/*     */   }
/*     */ 
/*     */   public Dimension maximumLayoutSize(Container pContainer)
/*     */   {
/* 175 */     return new Dimension(2147483647, 2147483647);
/*     */   }
/*     */ 
/*     */   public void addLayoutComponent(Component pComponent, Object pConstraints)
/*     */   {
/* 182 */     this.constraints.add(new Constraint(pComponent, pConstraints));
/*     */   }
/*     */ 
/*     */   public void removeLayoutComponent(Component pComponent)
/*     */   {
/* 190 */     for (int a = 0; a < this.constraints.size(); a++)
/*     */     {
/* 192 */       Constraint wConstraint = (Constraint)this.constraints.get(a);
/*     */ 
/* 194 */       if (wConstraint.component == pComponent) {
/* 195 */         this.constraints.remove(a);
/* 196 */         return;
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   public void layoutContainer(Container pContainer)
/*     */   {
/* 208 */     for (int a = 0; a < this.constraints.size(); a++)
/*     */     {
/* 210 */       Constraint wConstraint = (Constraint)this.constraints.get(a);
/*     */ 
/* 212 */       wConstraint.component.setBounds(wConstraint.bounds);
/*     */     }
/*     */   }
/*     */ 
/*     */   public void addLayoutComponent(String arg0, Component arg1)
/*     */   {
/*     */   }
/*     */ 
/*     */   public Dimension minimumLayoutSize(Container pContainer)
/*     */   {
/* 229 */     return getPreferredLayoutSize(pContainer);
/*     */   }
/*     */ 
/*     */   public Dimension preferredLayoutSize(Container pContainer)
/*     */   {
/* 236 */     return getPreferredLayoutSize(pContainer);
/*     */   }
/*     */ 
/*     */   private Dimension getPreferredLayoutSize(Container pContainer)
/*     */   {
/* 241 */     if (this.preferredSize == null)
/*     */     {
/* 243 */       if (this.constraints.isEmpty()) {
/* 244 */         return new Dimension(0, 0);
/*     */       }
/*     */ 
/* 247 */       Insets wInsets = pContainer.getInsets();
/*     */ 
/* 249 */       Constraint wConstraint = (Constraint)this.constraints.get(this.constraints.size() - 1);
/*     */ 
/* 251 */       if (wConstraint.bounds == null) {
/* 252 */         invalidateLayout(pContainer);
/*     */       }
/*     */ 
/* 255 */       Rectangle wBounds = wConstraint.bounds;
/*     */ 
/* 257 */       this.preferredSize = 
/* 259 */         new Dimension(wBounds.x + wBounds.width + wInsets.right, 
/* 259 */         wBounds.y + wBounds.height + wInsets.bottom);
/*     */     }
/*     */ 
/* 263 */     return this.preferredSize;
/*     */   }
/*     */ 
/*     */   private class Constraint
/*     */   {
/*     */     public Component component;
/*     */     public Integer type;
/*     */     public Rectangle bounds;
/*     */ 
/*     */     public Constraint(Component pComponent, Object pType) {
/* 276 */       this.component = pComponent;
/*     */       try
/*     */       {
/* 279 */         this.type = ((Integer)pType);
/*     */       } catch (ClassCastException pEx) {
/* 281 */         throw new IllegalArgumentException("Illegal constraint: " + pType);
/*     */       }
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJFlowLayout
 * JD-Core Version:    0.6.0
 */