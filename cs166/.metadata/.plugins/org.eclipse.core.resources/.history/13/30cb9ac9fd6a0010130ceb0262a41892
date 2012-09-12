/*     */ package kj.ui;
/*     */ 
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.HeadlessException;
/*     */ import java.awt.Rectangle;
/*     */ import java.awt.event.MouseAdapter;
/*     */ import java.awt.event.MouseEvent;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JFrame;
/*     */ import javax.swing.JMenu;
/*     */ import javax.swing.JMenuBar;
/*     */ import javax.swing.JMenuItem;
/*     */ import javax.swing.JOptionPane;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JPopupMenu;
/*     */ import javax.swing.JTable;
/*     */ import javax.swing.ListSelectionModel;
/*     */ import kj.player.KJPlayer;
/*     */ import kj.player.KJPlayerEvent;
/*     */ import kj.player.KJPlayerListener;
/*     */ import kj.track.KJTrack;
/*     */ import kj.track.KJTrackList;
/*     */ import kj.track.KJTrackProviderChangeEvent;
/*     */ import kj.track.KJTrackProviderChangeListener;
/*     */ import kj.venue.KJVenue;
/*     */ import kj.venue.KJVenueChangeListener;
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public class KJRequestListDialog extends JDialog
/*     */   implements KJPlayerListener, KJTrackProviderChangeListener, KJVenueChangeListener, KJXmlSerializable
/*     */ {
/*     */   private JTable requestListTable;
/*     */   private JPopupMenu requestListPopupMenu;
/*     */   private KJVenue venue;
/*     */ 
/*     */   public KJRequestListDialog(JFrame pParent)
/*     */     throws HeadlessException
/*     */   {
/*  53 */     super(pParent);
/*     */ 
/*  55 */     prepareGUI();
/*     */ 
/*  57 */     KJJukeBox.addVenueChangeListener(this);
/*  58 */     KJJukeBox.getPlayer().addListener(this);
/*     */   }
/*     */ 
/*     */   private KJTrackList getRequestList()
/*     */   {
/*  63 */     return this.venue.getRequestList();
/*     */   }
/*     */ 
/*     */   private KJTrack getSelectedTrack()
/*     */   {
/*  68 */     int wSr = this.requestListTable.getSelectedRow();
/*     */ 
/*  70 */     if (wSr != -1) {
/*  71 */       return (KJTrack)this.requestListTable.getValueAt(wSr, 0);
/*     */     }
/*     */ 
/*  74 */     return null;
/*     */   }
/*     */ 
/*     */   private void move(int[] pSelectedRows, int pDirection)
/*     */   {
/*  80 */     KJTrackList wCpl = getRequestList();
/*     */ 
/*  82 */     wCpl.moveTracks(pSelectedRows, pDirection);
/*     */ 
/*  84 */     int wLsr = 0;
/*     */ 
/*  86 */     if (pDirection < 0)
/*  87 */       wLsr = pSelectedRows[0];
/*     */     else {
/*  89 */       wLsr = pSelectedRows[(pSelectedRows.length - 1)];
/*     */     }
/*     */ 
/*  92 */     for (int a = 0; a < pSelectedRows.length; a++) {
/*  93 */       this.requestListTable.getSelectionModel().addSelectionInterval(
/*  94 */         pSelectedRows[a], 
/*  95 */         pSelectedRows[a]);
/*     */     }
/*     */ 
/*  98 */     this.requestListTable.scrollRectToVisible(this.requestListTable.getCellRect(wLsr, wLsr, false));
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/* 172 */     setTitle("KJ - Request List");
/*     */ 
/* 174 */     setSize(400, 400);
/* 175 */     setDefaultCloseOperation(1);
/*     */ 
/* 177 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*     */ 
/* 179 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/* 181 */     JPanel wDisplayPanel = new JPanel(new BorderLayout());
/* 182 */     getContentPane().add(wDisplayPanel, "Center");
/*     */ 
/* 184 */     wDisplayPanel.setBorder(KJUITools.createTitleBorder("Requested Tracks"));
/*     */ 
/* 186 */     JPanel wRequestListPanel = new JPanel(new BorderLayout());
/*     */ 
/* 188 */     wDisplayPanel.add(wRequestListPanel, "Center");
/*     */ 
/* 190 */     JPanel wTrackPanel = new JPanel(new BorderLayout());
/*     */ 
/* 192 */     wRequestListPanel.add(wTrackPanel, "Center");
/*     */ 
/* 194 */     this.requestListTable = KJUITools.createTrackListTable(true);
/*     */ 
/* 196 */     wTrackPanel.add(KJUITools.createScrollPane(this.requestListTable), "Center");
/*     */ 
/* 199 */     this.requestListTable.addMouseListener(new RequestListMouseListener(null));
/*     */ 
/* 201 */     prepareMenus();
/*     */   }
/*     */ 
/*     */   private void prepareMenus()
/*     */   {
/* 207 */     JMenuBar wMenuBar = new JMenuBar();
/*     */ 
/* 209 */     setJMenuBar(wMenuBar);
/*     */ 
/* 211 */     JMenu wRequestMenu = KJUITools.addMenu(wMenuBar, "Request List", 'R');
/*     */ 
/* 213 */     KJUITools.addMenuItem(wRequestMenu, KJUITools.createMenuItem("Preferrences...", 'P', this));
/* 214 */     wRequestMenu.addSeparator();
/* 215 */     KJUITools.addMenuItem(wRequestMenu, KJUITools.createMenuItem("Close", 'C', this));
/*     */ 
/* 217 */     JMenu wTrackMenu = KJUITools.addMenu(wMenuBar, "Tracks", 'R');
/*     */ 
/* 219 */     JMenu wMoveMenu = KJUITools.addMenu(wTrackMenu, "Move", 'M');
/*     */ 
/* 221 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Top", 'T', "alt HOME", this));
/* 222 */     wMoveMenu.addSeparator();
/* 223 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Up 10", 'P', "alt PAGE_UP", this));
/* 224 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Up", 'U', "alt UP", this));
/* 225 */     wMoveMenu.addSeparator();
/* 226 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Down", 'D', "alt DOWN", this));
/* 227 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Down 10", 'W', "alt PAGE_DOWN", this));
/* 228 */     wMoveMenu.addSeparator();
/* 229 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Bottom", 'B', "alt END", this));
/*     */ 
/* 231 */     wTrackMenu.addSeparator();
/*     */ 
/* 233 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Remove", 'R', "DELETE", this));
/* 234 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Remove All", 'A', "shift DELETE", this));
/* 235 */     wTrackMenu.addSeparator();
/* 236 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Show in Track List Editor", 'S', "ctrl SPACE", this));
/* 237 */     wTrackMenu.addSeparator();
/* 238 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Properties...", 'P', "ctrl P", this));
/*     */ 
/* 240 */     this.requestListPopupMenu = new JPopupMenu("Tracks");
/*     */ 
/* 242 */     KJUITools.addMenuItem(this.requestListPopupMenu, KJUITools.createMenuItem("Show in Track List Editor", 'S', this));
/* 243 */     this.requestListPopupMenu.addSeparator();
/* 244 */     KJUITools.addMenuItem(this.requestListPopupMenu, KJUITools.createMenuItem("Remove", 'R', this));
/* 245 */     this.requestListPopupMenu.addSeparator();
/* 246 */     KJUITools.addMenuItem(this.requestListPopupMenu, KJUITools.createMenuItem("Properties...", 'P', this));
/*     */   }
/*     */ 
/*     */   public void playerEvent(KJPlayerEvent pEvent)
/*     */   {
/* 261 */     refreshRequestList();
/*     */   }
/*     */ 
/*     */   public void venueChanged(KJVenue pVenue)
/*     */   {
/* 266 */     if (this.venue != null) {
/* 267 */       this.venue.removeListener(this);
/*     */     }
/*     */ 
/* 270 */     this.venue = pVenue;
/*     */ 
/* 272 */     ((KJTrackListTableModel)this.requestListTable.getModel()).setTrackList(this.venue.getRequestList());
/*     */ 
/* 274 */     this.venue.addListener(this);
/*     */   }
/*     */ 
/*     */   private void miRequestListPreferrences(JMenuItem pMenuItem)
/*     */   {
/* 279 */     new KJRequestListPreferrences(this, this.venue);
/*     */   }
/*     */ 
/*     */   private void miRequestListClose(JMenuItem pMenuItem) {
/* 283 */     setVisible(false);
/*     */   }
/*     */ 
/*     */   private void miTracksRemove(JMenuItem pMenuItem)
/*     */   {
/* 288 */     int[] wSr = this.requestListTable.getSelectedRows();
/* 289 */     int wDo = 0;
/*     */ 
/* 291 */     for (int a = 0; a < wSr.length; a++)
/*     */     {
/* 293 */       KJTrack wTrack = getRequestList().removeTrack(wSr[a] - wDo);
/*     */ 
/* 295 */       wTrack.setRequested(false);
/*     */ 
/* 297 */       wDo++;
/*     */     }
/*     */ 
/* 301 */     refreshRequestList();
/*     */ 
/* 303 */     KJJukeBox.getPlayListEditorDialog().refreshTrackList();
/*     */   }
/*     */ 
/*     */   private void miTracksRemoveAll(JMenuItem pMenuItem)
/*     */   {
/* 309 */     int wRc = JOptionPane.showConfirmDialog(
/* 310 */       this, 
/* 311 */       "Remove all tracks from request list?", 
/* 312 */       "Remove All", 
/* 313 */       0);
/*     */ 
/* 315 */     if (wRc == 0)
/*     */     {
/* 317 */       for (int a = 0; a < getRequestList().size(); a++) {
/* 318 */         getRequestList().getTrack(a).setRequested(false);
/*     */       }
/*     */ 
/* 321 */       getRequestList().clear();
/* 322 */       refreshRequestList();
/*     */ 
/* 324 */       KJJukeBox.getPlayListEditorDialog().refreshTrackList();
/*     */     }
/*     */   }
/*     */ 
/*     */   private void miTracksShowinPlaylistEditor(JMenuItem pMenuItem)
/*     */   {
/* 332 */     KJTrack wSelectedTrack = getSelectedTrack();
/*     */ 
/* 334 */     if (wSelectedTrack != null)
/* 335 */       KJJukeBox.getPlayListEditorDialog().showTrack(wSelectedTrack);
/*     */   }
/*     */ 
/*     */   public void refreshRequestList()
/*     */   {
/* 341 */     ((KJListModel)this.requestListTable.getModel()).update();
/*     */   }
/*     */ 
/*     */   private void showTrackProperties(JTable pTrackTable)
/*     */   {
/* 346 */     int[] wSrs = pTrackTable.getSelectedRows();
/* 347 */     KJTrack[] wSelectedTracks = new KJTrack[wSrs.length];
/*     */ 
/* 349 */     for (int a = 0; a < wSrs.length; a++) {
/* 350 */       wSelectedTracks[a] = this.venue.getRequestList().getTrack(wSrs[a]);
/*     */     }
/*     */ 
/* 353 */     new KJTrackProperties(this, wSelectedTracks);
/*     */   }
/*     */ 
/*     */   private void miMoveTop(JMenuItem pMenuItem)
/*     */   {
/* 358 */     move(this.requestListTable.getSelectedRows(), -getRequestList().size());
/*     */   }
/*     */ 
/*     */   private void miMoveUp(JMenuItem pMenuItem) {
/* 362 */     move(this.requestListTable.getSelectedRows(), -1);
/*     */   }
/*     */ 
/*     */   private void miMoveDown(JMenuItem pMenuItem) {
/* 366 */     move(this.requestListTable.getSelectedRows(), 1);
/*     */   }
/*     */ 
/*     */   private void miMoveBottom(JMenuItem pMenuItem) {
/* 370 */     move(this.requestListTable.getSelectedRows(), getRequestList().size());
/*     */   }
/*     */ 
/*     */   private void miMoveUp10(JMenuItem pMenuItem) {
/* 374 */     move(this.requestListTable.getSelectedRows(), -10);
/*     */   }
/*     */ 
/*     */   private void miMoveDown10(JMenuItem pMenuItem) {
/* 378 */     move(this.requestListTable.getSelectedRows(), 10);
/*     */   }
/*     */ 
/*     */   private void miTracksProperties(JMenuItem pMenuItem)
/*     */   {
/* 383 */     int[] wSrs = this.requestListTable.getSelectedRows();
/* 384 */     KJTrack[] wSelectedTracks = new KJTrack[wSrs.length];
/*     */ 
/* 386 */     KJTrackList wSpl = getRequestList();
/*     */ 
/* 388 */     for (int a = 0; a < wSrs.length; a++) {
/* 389 */       wSelectedTracks[a] = wSpl.getTrack(wSrs[a]);
/*     */     }
/*     */ 
/* 392 */     if (wSelectedTracks.length > 0)
/* 393 */       new KJTrackProperties(this, wSelectedTracks);
/*     */   }
/*     */ 
/*     */   public void trackProviderChanged(KJTrackProviderChangeEvent pEvent)
/*     */   {
/* 404 */     refreshRequestList();
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource)
/*     */   {
/* 412 */     Element wBounds = KJXmlToolkit.find(pSource, "Bounds");
/*     */ 
/* 414 */     if (wBounds != null)
/*     */     {
/* 416 */       setBounds(
/* 417 */         Integer.parseInt(wBounds.getAttribute("x")), 
/* 418 */         Integer.parseInt(wBounds.getAttribute("y")), 
/* 419 */         Integer.parseInt(wBounds.getAttribute("width")), 
/* 420 */         Integer.parseInt(wBounds.getAttribute("height")));
/*     */     }
/*     */ 
/* 424 */     setVisible(KJXmlToolkit.get(pSource, "Visible", true));
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 433 */     Element wBounds = KJXmlToolkit.add(pTarget, "Bounds");
/*     */ 
/* 435 */     wBounds.setAttribute("x", String.valueOf(getBounds().x));
/* 436 */     wBounds.setAttribute("y", String.valueOf(getBounds().y));
/* 437 */     wBounds.setAttribute("width", String.valueOf(getBounds().width));
/* 438 */     wBounds.setAttribute("height", String.valueOf(getBounds().height));
/*     */ 
/* 440 */     KJXmlToolkit.set(pTarget, "Visible", isVisible());
/*     */   }
/*     */ 
/*     */   private class RequestListMouseListener extends MouseAdapter {
/*     */     private RequestListMouseListener() {
/*     */     }
/*     */ 
/*     */     public void mouseClicked(MouseEvent pEvent) {
/* 448 */       if (pEvent.getButton() == 3)
/*     */       {
/* 452 */         int wRap = KJRequestListDialog.this.requestListTable.rowAtPoint(pEvent.getPoint());
/*     */ 
/* 454 */         if (!KJRequestListDialog.this.requestListTable.isRowSelected(wRap)) {
/* 455 */           KJRequestListDialog.this.requestListTable.setRowSelectionInterval(wRap, wRap);
/*     */         }
/*     */ 
/* 458 */         KJRequestListDialog.this.requestListPopupMenu.show(KJRequestListDialog.this.requestListTable, pEvent.getX(), pEvent.getY());
/*     */       }
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJRequestListDialog
 * JD-Core Version:    0.6.0
 */