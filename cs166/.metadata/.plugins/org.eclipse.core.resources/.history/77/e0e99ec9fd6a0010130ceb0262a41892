/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.Component;
/*     */ import java.awt.Container;
/*     */ import java.awt.Cursor;
/*     */ import java.awt.Dialog;
/*     */ import java.awt.Dimension;
/*     */ import java.awt.Point;
/*     */ import java.awt.SystemColor;
/*     */ import java.awt.Toolkit;
/*     */ import java.awt.Window;
/*     */ import java.awt.event.ActionEvent;
/*     */ import java.awt.event.ActionListener;
/*     */ import java.awt.event.ItemEvent;
/*     */ import java.awt.event.ItemListener;
/*     */ import java.io.File;
/*     */ import java.lang.reflect.InvocationTargetException;
/*     */ import java.lang.reflect.Method;
/*     */ import java.util.EventObject;
/*     */ import javax.swing.AbstractButton;
/*     */ import javax.swing.BorderFactory;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JCheckBoxMenuItem;
/*     */ import javax.swing.JComboBox;
/*     */ import javax.swing.JFileChooser;
/*     */ import javax.swing.JMenu;
/*     */ import javax.swing.JMenuBar;
/*     */ import javax.swing.JMenuItem;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JPopupMenu;
/*     */ import javax.swing.JScrollPane;
/*     */ import javax.swing.JTable;
/*     */ import javax.swing.JViewport;
/*     */ import javax.swing.KeyStroke;
/*     */ import javax.swing.border.Border;
/*     */ import javax.swing.filechooser.FileFilter;
/*     */ import kj.track.KJTrack;
/*     */ 
/*     */ public class KJUITools
/*     */ {
/*  56 */   private static final Object invokeLock = new Object();
/*  57 */   private static boolean busy = false;
/*     */ 
/*  59 */   private static File lastBrowseLocation = new File(System.getProperty("user.home"));
/*     */ 
/*     */   public static JMenu addMenu(JMenuBar pMenuBar, String pLabel, char pMnemonic)
/*     */   {
/*  63 */     JMenu wMenu = pMenuBar.add(new JMenu(pLabel));
/*     */ 
/*  65 */     wMenu.setMnemonic(pMnemonic);
/*     */ 
/*  67 */     return wMenu;
/*     */   }
/*     */ 
/*     */   public static JMenu addMenu(JMenu pMenu, String pLabel, char pMnemonic)
/*     */   {
/*  73 */     JMenu wMenu = (JMenu)pMenu.add(new JMenu(pLabel));
/*     */ 
/*  75 */     wMenu.setMnemonic(pMnemonic);
/*     */ 
/*  77 */     return wMenu;
/*     */   }
/*     */ 
/*     */   public static JMenuItem addMenuItem(JMenu pMenu, JMenuItem pItem)
/*     */   {
/*  83 */     JMenuItem wMi = pMenu.add(pItem);
/*     */ 
/*  85 */     wMi.setActionCommand(toActionCommand(pMenu.getText() + pItem.getText()));
/*     */ 
/*  87 */     return wMi;
/*     */   }
/*     */ 
/*     */   public static JMenuItem addMenuItem(JPopupMenu pPopupMenu, JMenuItem pItem)
/*     */   {
/*  93 */     JMenuItem wMi = pPopupMenu.add(pItem);
/*     */ 
/*  95 */     wMi.setActionCommand(toActionCommand(pPopupMenu.getLabel() + pItem.getText()));
/*     */ 
/*  97 */     return wMi;
/*     */   }
/*     */ 
/*     */   public static JButton createButton(String pLabel, Object pActionObjectTarget)
/*     */   {
/* 102 */     return createButton(pLabel, null, pActionObjectTarget);
/*     */   }
/*     */ 
/*     */   public static JMenuItem createCheckMenuItem(String pLabel, char pMnemonic, Object pActionObject) {
/* 106 */     return createMenuItem(pLabel, pMnemonic, null, pActionObject, true);
/*     */   }
/*     */ 
/*     */   public static JMenuItem createCheckMenuItem(String pLabel, char pMnemonic, String pAccelerator, Object pActionObject) {
/* 110 */     return createMenuItem(pLabel, pMnemonic, pAccelerator, pActionObject, true);
/*     */   }
/*     */ 
/*     */   public static JMenuItem createMenuItem(String pLabel, char pMnemonic, Object pActionObject) {
/* 114 */     return createMenuItem(pLabel, pMnemonic, null, pActionObject, false);
/*     */   }
/*     */ 
/*     */   public static JMenuItem createMenuItem(String pLabel, char pMnemonic, String pAccelerator, Object pActionObject) {
/* 118 */     return createMenuItem(pLabel, pMnemonic, pAccelerator, pActionObject, false);
/*     */   }
/*     */ 
/*     */   private static JMenuItem createMenuItem(String pLabel, char pMnemonic, String pAccelerator, Object pActionObject, boolean pCheckItem)
/*     */   {
/* 123 */     JMenuItem wMi = pCheckItem ? new JCheckBoxMenuItem(pLabel) : new JMenuItem(pLabel);
/*     */ 
/* 125 */     wMi.setMnemonic(pMnemonic);
/*     */ 
/* 127 */     if (pAccelerator != null) {
/* 128 */       wMi.setAccelerator(KeyStroke.getKeyStroke(pAccelerator));
/*     */     }
/*     */ 
/* 131 */     if (pCheckItem)
/* 132 */       wMi.addItemListener(new ItemListenerWithObjectTarget(pActionObject, "mc"));
/*     */     else {
/* 134 */       wMi.addActionListener(new ActionListenerWithObjectTarget(pActionObject, "mi"));
/*     */     }
/*     */ 
/* 137 */     return wMi;
/*     */   }
/*     */ 
/*     */   public static Point calculateCenterScreenPosition(Dimension pDialogSize)
/*     */   {
/* 143 */     Dimension wSd = Toolkit.getDefaultToolkit().getScreenSize();
/*     */ 
/* 145 */     return new Point(
/* 146 */       wSd.width - pDialogSize.width >> 1, 
/* 147 */       wSd.height - pDialogSize.height >> 1);
/*     */   }
/*     */ 
/*     */   public static JButton createButton(String pLabel, String pGroup, Object pActionObjectTarget)
/*     */   {
/* 153 */     JButton wButton = new JButton(pLabel);
/*     */ 
/* 155 */     Dimension wPs = wButton.getPreferredSize();
/*     */ 
/* 157 */     wPs.height = 19;
/*     */ 
/* 159 */     wButton.setPreferredSize(wPs);
/*     */ 
/* 161 */     if (pGroup != null)
/* 162 */       wButton.setActionCommand(pGroup + toActionCommand(pLabel));
/*     */     else {
/* 164 */       wButton.setActionCommand(toActionCommand(pLabel));
/*     */     }
/*     */ 
/* 167 */     wButton.addActionListener(
/* 168 */       new ActionListenerWithObjectTarget(
/* 169 */       pActionObjectTarget, 
/* 170 */       "btn"));
/*     */ 
/* 172 */     return wButton;
/*     */   }
/*     */ 
/*     */   public static JComboBox createComboBox(String pTargetMethodName, Object pActionObjectTarget)
/*     */   {
/* 178 */     JComboBox wComboBox = new JComboBox();
/*     */ 
/* 180 */     Dimension wPs = wComboBox.getPreferredSize();
/*     */ 
/* 182 */     wPs.height = 19;
/*     */ 
/* 184 */     wComboBox.setPreferredSize(wPs);
/*     */ 
/* 186 */     wComboBox.addItemListener(
/* 187 */       new ItemListenerWithObjectTarget(
/* 188 */       pActionObjectTarget, 
/* 189 */       pTargetMethodName));
/*     */ 
/* 191 */     return wComboBox;
/*     */   }
/*     */ 
/*     */   public static File[] createFileChooser(Component pParent, String pTitle, FileFilter pFilter)
/*     */   {
/* 196 */     return createFileChooser(pParent, pTitle, 0, true, pFilter);
/*     */   }
/*     */ 
/*     */   public static File[] createFileChooser(Component pParent, String pTitle, int pSelectionMode, boolean pMultipleSelection, FileFilter pFilter)
/*     */   {
/* 201 */     JFileChooser wFc = new JFileChooser(lastBrowseLocation);
/*     */ 
/* 203 */     wFc.setFileSelectionMode(pSelectionMode);
/* 204 */     wFc.setMultiSelectionEnabled(pMultipleSelection);
/*     */ 
/* 206 */     if (pFilter != null) {
/* 207 */       wFc.setFileFilter(pFilter);
/*     */     }
/*     */ 
/* 210 */     int wRc = wFc.showDialog(pParent, pTitle);
/*     */ 
/* 212 */     lastBrowseLocation = wFc.getCurrentDirectory();
/*     */ 
/* 214 */     if (wRc == 0) {
/* 215 */       if (pMultipleSelection) {
/* 216 */         return wFc.getSelectedFiles();
/*     */       }
/* 218 */       return new File[] { wFc.getSelectedFile() };
/*     */     }
/*     */ 
/* 222 */     return null;
/*     */   }
/*     */ 
/*     */   public static File createFolderChooser(Component pParent, String pTitle)
/*     */   {
/* 228 */     File[] wSf = createFileChooser(pParent, pTitle, 1, false, null);
/*     */ 
/* 230 */     if ((wSf != null) && (wSf.length > 0)) {
/* 231 */       return wSf[0];
/*     */     }
/*     */ 
/* 234 */     return null;
/*     */   }
/*     */ 
/*     */   public static File createSingleFileChooser(Component pParent, String pTitle, FileFilter pFilter)
/*     */   {
/* 240 */     File[] wSf = createFileChooser(pParent, pTitle, 0, false, pFilter);
/*     */ 
/* 242 */     if ((wSf != null) && (wSf.length > 0)) {
/* 243 */       return wSf[0];
/*     */     }
/*     */ 
/* 246 */     return null;
/*     */   }
/*     */ 
/*     */   public static JTable createPlayListTable()
/*     */   {
/* 251 */     return createTrackListTable(true);
/*     */   }
/*     */ 
/*     */   public static JTable createTrackListTable(boolean pDisplayEntryStatusColors)
/*     */   {
/* 256 */     JTable wPlt = createList();
/*     */ 
/* 258 */     wPlt.setSelectionMode(2);
/* 259 */     wPlt.setDefaultRenderer(KJTrack.class, new KJPlayListEntryRenderer(pDisplayEntryStatusColors));
/* 260 */     wPlt.setModel(new KJTrackListTableModel());
/* 261 */     wPlt.setIntercellSpacing(new Dimension(0, 0));
/* 262 */     wPlt.setFocusable(true);
/*     */ 
/* 264 */     return wPlt;
/*     */   }
/*     */ 
/*     */   public static JTable createList()
/*     */   {
/* 270 */     JTable wPlt = new JTable();
/*     */ 
/* 272 */     wPlt.setTableHeader(null);
/* 273 */     wPlt.setShowGrid(false);
/* 274 */     wPlt.setSelectionMode(0);
/*     */ 
/* 276 */     return wPlt;
/*     */   }
/*     */ 
/*     */   public static void createVerticalButtonCluster(JPanel pContainer, String[] pLabels, Object pActionObjectTarget)
/*     */   {
/* 281 */     createVerticalButtonCluster(pContainer, pLabels, null, pActionObjectTarget);
/*     */   }
/*     */ 
/*     */   public static void createVerticalButtonCluster(JPanel pContainer, String[] pLabels, String pGroup, Object pActionObjectTarget)
/*     */   {
/* 286 */     pContainer.setLayout(null);
/*     */ 
/* 288 */     int y = 4;
/* 289 */     int wMw = 0;
/*     */ 
/* 291 */     for (int a = 0; a < pLabels.length; a++)
/*     */     {
/* 293 */       if (pLabels[a] != null)
/*     */       {
/* 295 */         JButton wButton = (JButton)pContainer.add(
/* 296 */           createButton(
/* 297 */           pLabels[a], 
/* 298 */           pGroup, 
/* 299 */           pActionObjectTarget));
/*     */ 
/* 301 */         wButton.setLocation(3, y);
/*     */ 
/* 303 */         Dimension wPs = wButton.getPreferredSize();
/*     */ 
/* 306 */         if (wMw < wPs.width) {
/* 307 */           wMw = wPs.width;
/*     */         }
/*     */ 
/* 310 */         y += 20;
/*     */       }
/*     */       else {
/* 313 */         y += 10;
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 319 */     for (int a = 0; a < pContainer.getComponentCount(); a++)
/*     */     {
/* 321 */       Component wButton = pContainer.getComponent(a);
/*     */ 
/* 323 */       Dimension wPs = wButton.getPreferredSize();
/*     */ 
/* 325 */       wButton.setSize(wMw, wPs.height);
/*     */     }
/*     */ 
/* 329 */     pContainer.setPreferredSize(new Dimension(wMw + 4, y + 4));
/*     */   }
/*     */ 
/*     */   public static JScrollPane createScrollPane(Component pComponent)
/*     */   {
/* 335 */     JScrollPane wSp = new JScrollPane(pComponent);
/*     */ 
/* 337 */     wSp.getViewport().setBackground(SystemColor.window);
/*     */ 
/* 340 */     return wSp;
/*     */   }
/*     */ 
/*     */   public static Border createTitleBorder(String pTitle)
/*     */   {
/* 346 */     return BorderFactory.createCompoundBorder(
/* 347 */       BorderFactory.createTitledBorder(pTitle), 
/* 348 */       BorderFactory.createEmptyBorder(0, 10, 10, 10));
/*     */   }
/*     */ 
/*     */   public static void setEnabled(Container pContainer, boolean pState)
/*     */   {
/* 354 */     for (int a = 0; a < pContainer.getComponentCount(); a++)
/*     */     {
/* 356 */       Component wComponent = pContainer.getComponent(a);
/*     */ 
/* 358 */       if ((wComponent instanceof Container)) {
/* 359 */         setEnabled((Container)wComponent, pState);
/*     */       }
/*     */ 
/* 362 */       wComponent.setEnabled(pState);
/*     */     }
/*     */   }
/*     */ 
/*     */   public static File getFileChooserBrowseLocation()
/*     */   {
/* 369 */     return lastBrowseLocation;
/*     */   }
/*     */ 
/*     */   public static void setFileChooserBrowseLocation(File pLocation)
/*     */   {
/* 391 */     lastBrowseLocation = pLocation;
/*     */   }
/*     */ 
/*     */   public static String toActionCommand(String pLabel)
/*     */   {
/* 396 */     StringBuffer wAc = new StringBuffer();
/*     */ 
/* 398 */     for (int a = 0; a < pLabel.length(); a++)
/*     */     {
/* 400 */       if (Character.isLetterOrDigit(pLabel.charAt(a))) {
/* 401 */         wAc.append(pLabel.charAt(a));
/*     */       }
/*     */ 
/*     */     }
/*     */ 
/* 406 */     return wAc.toString();
/*     */   }
/*     */ 
/*     */   private static class ActionListenerWithObjectTarget extends KJUITools.ListenerWithObjectTarget implements ActionListener
/*     */   {
/*     */     public ActionListenerWithObjectTarget(Object pActionObjectTarget, String pPrefix)
/*     */     {
/* 413 */       super(pPrefix);
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent pEvent) {
/* 417 */       forwardEventToActionObjectTargetMethod(pEvent);
/*     */     }
/*     */   }
/*     */ 
/*     */   private static class ItemListenerWithObjectTarget extends KJUITools.ListenerWithObjectTarget implements ItemListener
/*     */   {
/*     */     public ItemListenerWithObjectTarget(Object pActionObjectTarget, String pPrefix)
/*     */     {
/* 425 */       super(pPrefix);
/*     */     }
/*     */ 
/*     */     public void itemStateChanged(ItemEvent pEvent) {
/* 429 */       forwardEventToActionObjectTargetMethod(pEvent);
/*     */     }
/*     */   }
/*     */ 
/*     */   private static class ListenerWithObjectTarget
/*     */   {
/*     */     private Object actionObjectTarget;
/*     */     private String prefix;
/* 439 */     private boolean block = true;
/*     */ 
/*     */     public ListenerWithObjectTarget(Object pActionObjectTarget, String pPrefix)
/*     */     {
/* 443 */       this.actionObjectTarget = pActionObjectTarget;
/* 444 */       this.prefix = pPrefix;
/*     */ 
/* 446 */       if ((this.actionObjectTarget instanceof Dialog))
/* 447 */         this.block = (!((Dialog)this.actionObjectTarget).isModal());
/*     */     }
/*     */ 
/*     */     public void forwardEventToActionObjectTargetMethod(EventObject pEvent)
/*     */     {
/* 454 */       synchronized (KJUITools.invokeLock)
/*     */       {
/* 456 */         if ((KJUITools.busy) && (this.block))
/*     */         {
/* 458 */           return;
/*     */         }
/*     */ 
/* 461 */         KJUITools.busy = true;
/*     */ 
/* 463 */         if ((this.actionObjectTarget instanceof Window)) {
/* 464 */           ((Window)this.actionObjectTarget).setCursor(Cursor.getPredefinedCursor(3));
/*     */         }
/*     */ 
/* 467 */         Thread wThread = new Thread(
/* 468 */           new Runnable(pEvent)
/*     */         {
/*     */           public void run() {
/* 471 */             AbstractButton wAb = (AbstractButton)this.val$pEvent.getSource();
/*     */ 
/* 473 */             String wTargetMethodName = KJUITools.ListenerWithObjectTarget.this.prefix + wAb.getActionCommand();
/*     */             try
/*     */             {
/* 477 */               Method wMethod = KJUITools.ListenerWithObjectTarget.this.actionObjectTarget.getClass().getDeclaredMethod(
/* 478 */                 wTargetMethodName, 
/* 479 */                 new Class[] { this.val$pEvent.getSource().getClass() });
/*     */ 
/* 481 */               wMethod.setAccessible(true);
/*     */ 
/* 483 */               wMethod.invoke(
/* 484 */                 KJUITools.ListenerWithObjectTarget.this.actionObjectTarget, 
/* 485 */                 new Object[] { this.val$pEvent.getSource() });
/*     */             }
/*     */             catch (InvocationTargetException pEx) {
/* 488 */               pEx.printStackTrace();
/*     */             }
/*     */             catch (Exception localException)
/*     */             {
/*     */             }
/* 493 */             if ((KJUITools.ListenerWithObjectTarget.this.actionObjectTarget instanceof Window)) {
/* 494 */               ((Window)KJUITools.ListenerWithObjectTarget.this.actionObjectTarget).setCursor(Cursor.getDefaultCursor());
/*     */             }
/*     */ 
/* 497 */             synchronized (KJUITools.invokeLock) {
/* 498 */               KJUITools.busy = false;
/*     */             }
/*     */           }
/*     */         });
/* 508 */         wThread.setPriority(1);
/*     */ 
/* 510 */         wThread.start();
/*     */       }
/*     */     }
/*     */   }
/*     */ 
/*     */   public static class FileFilterMPEG1Layer3 extends FileFilter
/*     */   {
/*     */     public boolean accept(File pFile)
/*     */     {
/* 521 */       return (pFile.isDirectory()) || (pFile.getName().endsWith(".mp3"));
/*     */     }
/*     */ 
/*     */     public String getDescription() {
/* 525 */       return "MPEG-1 Layer 3 (*.mp3)";
/*     */     }
/*     */   }
/*     */ 
/*     */   public static class FileFilterMP3andMPS extends FileFilter
/*     */   {
/*     */     public boolean accept(File pFile)
/*     */     {
/* 538 */       return (pFile.isDirectory()) || (pFile.getName().endsWith(".mp3")) || (pFile.getName().endsWith(".mpS"));
/*     */     }
/*     */ 
/*     */     public String getDescription() {
/* 542 */       return "MPEG-1 Layer 3 possibly with SecureEncryption (*.mp3 OR *.mpS)";
/*     */     }
/*     */   }
/*     */ 
/*     */   public static class FileFilterWinampPlayList extends FileFilter
/*     */   {
/*     */     public boolean accept(File pFile) {
/* 549 */       return (pFile.isDirectory()) || (pFile.getName().endsWith(".m3u"));
/*     */     }
/*     */ 
/*     */     public String getDescription() {
/* 553 */       return "Winamp Play List (*.m3u)";
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJUITools
 * JD-Core Version:    0.6.0
 */