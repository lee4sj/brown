/*      */ package kj.ui;
/*      */ 
/*      */ import java.awt.BorderLayout;
/*      */ import java.awt.Container;
/*      */ import java.awt.EventQueue;
/*      */ import java.awt.HeadlessException;
/*      */ import java.awt.Rectangle;
/*      */ import java.awt.event.ItemEvent;
/*      */ import java.awt.event.ItemListener;
/*      */ import java.awt.event.MouseAdapter;
/*      */ import java.awt.event.MouseEvent;
/*      */ import java.io.BufferedReader;
/*      */ import java.io.File;
/*      */ import java.io.FileReader;
/*      */ import java.io.IOException;
/*      */ import java.util.ArrayList;
/*      */ import javax.swing.ComboBoxModel;
/*      */ import javax.swing.JCheckBox;
/*      */ import javax.swing.JCheckBoxMenuItem;
/*      */ import javax.swing.JComboBox;
/*      */ import javax.swing.JDialog;
/*      */ import javax.swing.JMenu;
/*      */ import javax.swing.JMenuBar;
/*      */ import javax.swing.JMenuItem;
/*      */ import javax.swing.JOptionPane;
/*      */ import javax.swing.JPanel;
/*      */ import javax.swing.JPopupMenu;
/*      */ import javax.swing.JTable;
/*      */ import javax.swing.ListSelectionModel;
/*      */ import javax.swing.event.ListDataEvent;
/*      */ import javax.swing.event.ListDataListener;
/*      */ import kj.player.KJPlayer;
/*      */ import kj.player.KJPlayerEvent;
/*      */ import kj.player.KJPlayerListener;
/*      */ import kj.track.KJMP3Track;
/*      */ import kj.track.KJTrack;
/*      */ import kj.track.KJTrackList;
/*      */ import kj.track.KJTrackProviderChangeEvent;
/*      */ import kj.track.KJTrackProviderChangeListener;
/*      */ import kj.venue.KJVenue;
/*      */ import kj.venue.KJVenue.SearchResults;
/*      */ import kj.venue.KJVenueChangeListener;
/*      */ import kj.xml.KJXmlSerializable;
/*      */ import kj.xml.KJXmlToolkit;
/*      */ import org.w3c.dom.Element;
/*      */ 
/*      */ public class KJEditTrackListsDialog extends JDialog
/*      */   implements KJVenueChangeListener, KJPlayerListener, KJXmlSerializable
/*      */ {
/*      */   private JComboBox trackListSelector;
/*      */   private JCheckBox activeTrackList;
/*      */   private JTable trackListTable;
/*      */   private JPopupMenu trackListPopupMenu;
/*      */   private JCheckBoxMenuItem trackPickModeSingle;
/*      */   private JCheckBoxMenuItem trackPickModeSequencial;
/*      */   private JCheckBoxMenuItem trackPickModeRandom;
/*      */   private JCheckBoxMenuItem trackPickModeProportional;
/*      */   private KJVenue venue;
/*   73 */   private boolean ignoreStateChange = false;
/*      */   private KJJukeBox juke_box;
/*      */   private TrackListModel trackListSelectorModel;
/*      */ 
/*      */   public KJEditTrackListsDialog(KJJukeBox pParent)
/*      */     throws HeadlessException
/*      */   {
/*   81 */     super(pParent);
/*      */ 
/*   83 */     this.juke_box = pParent;
/*      */ 
/*   85 */     prepareGUI();
/*      */ 
/*   87 */     KJJukeBox.addVenueChangeListener(this);
/*   88 */     KJJukeBox.getPlayer().addListener(this);
/*      */   }
/*      */ 
/*      */   private KJTrackList getSelectedTrackList() {
/*   92 */     return (KJTrackList)this.trackListSelector.getSelectedItem();
/*      */   }
/*      */ 
/*      */   private KJTrack getSelectedTrack()
/*      */   {
/*   97 */     int wSr = this.trackListTable.getSelectedRow();
/*      */ 
/*   99 */     if (wSr != -1) {
/*  100 */       return (KJTrack)this.trackListTable.getValueAt(wSr, 0);
/*      */     }
/*      */ 
/*  103 */     return null;
/*      */   }
/*      */ 
/*      */   private int getSelectedTrackIndex()
/*      */   {
/*  108 */     return this.trackListTable.getSelectedRow();
/*      */   }
/*      */ 
/*      */   private void move(int[] pSelectedRows, int pDirection)
/*      */   {
/*  113 */     KJTrackList wCpl = getSelectedTrackList();
/*      */ 
/*  115 */     wCpl.moveTracks(pSelectedRows, pDirection);
/*      */ 
/*  117 */     int wLsr = 0;
/*      */ 
/*  119 */     if (pDirection < 0)
/*  120 */       wLsr = pSelectedRows[0];
/*      */     else {
/*  122 */       wLsr = pSelectedRows[(pSelectedRows.length - 1)];
/*      */     }
/*      */ 
/*  125 */     for (int a = 0; a < pSelectedRows.length; a++) {
/*  126 */       this.trackListTable.getSelectionModel().addSelectionInterval(
/*  127 */         pSelectedRows[a], 
/*  128 */         pSelectedRows[a]);
/*      */     }
/*      */ 
/*  131 */     this.trackListTable.scrollRectToVisible(
/*  132 */       this.trackListTable.getCellRect(wLsr, wLsr, false));
/*      */   }
/*      */ 
/*      */   private void prepareGUI()
/*      */   {
/*  137 */     setTitle("KJ - Track List Editor");
/*      */ 
/*  139 */     setSize(380, 500);
/*  140 */     setDefaultCloseOperation(1);
/*      */ 
/*  142 */     setLocation(KJUITools.calculateCenterScreenPosition(getSize()));
/*      */ 
/*  144 */     getContentPane().setLayout(new BorderLayout());
/*      */ 
/*  146 */     JPanel wDisplayPanel = new JPanel(new BorderLayout());
/*  147 */     getContentPane().add(wDisplayPanel, "Center");
/*      */ 
/*  149 */     wDisplayPanel.setBorder(KJUITools.createTitleBorder("Track Lists"));
/*      */ 
/*  151 */     JPanel wTrackListTopPanel = new JPanel(new BorderLayout());
/*  152 */     wDisplayPanel.add(wTrackListTopPanel, "North");
/*      */ 
/*  154 */     this.trackListSelector = new JComboBox();
/*  155 */     this.trackListSelector.addItemListener(new TrackListChangeListener(null));
/*      */ 
/*  157 */     this.trackListSelectorModel = new TrackListModel(null);
/*  158 */     this.trackListSelector.setModel(this.trackListSelectorModel);
/*      */ 
/*  160 */     wTrackListTopPanel.add(this.trackListSelector, "Center");
/*      */ 
/*  162 */     this.activeTrackList = new JCheckBox("Active");
/*      */ 
/*  164 */     this.activeTrackList.addItemListener(new TrackListActivationListener(null));
/*  165 */     this.activeTrackList.setEnabled(false);
/*      */ 
/*  167 */     wTrackListTopPanel.add(this.activeTrackList, "East");
/*      */ 
/*  169 */     JPanel wTrackListPanel = new JPanel(new BorderLayout());
/*      */ 
/*  171 */     wDisplayPanel.add(wTrackListPanel, "Center");
/*      */ 
/*  173 */     JPanel wTrackPanel = new JPanel(new BorderLayout());
/*      */ 
/*  175 */     wTrackListPanel.add(wTrackPanel, "Center");
/*      */ 
/*  177 */     this.trackListTable = KJUITools.createTrackListTable(true);
/*      */ 
/*  179 */     wTrackPanel.add(KJUITools.createScrollPane(this.trackListTable), "Center");
/*      */ 
/*  181 */     this.trackListTable.addMouseListener(new TrackListMouseListener(null));
/*      */ 
/*  183 */     prepareMenus();
/*      */   }
/*      */ 
/*      */   private void prepareMenus()
/*      */   {
/*  189 */     JMenuBar wMenuBar = new JMenuBar();
/*      */ 
/*  191 */     setJMenuBar(wMenuBar);
/*      */ 
/*  193 */     JMenu wTrackListMenu = KJUITools.addMenu(wMenuBar, "Track Lists", 'P');
/*      */ 
/*  195 */     KJUITools.addMenuItem(wTrackListMenu, KJUITools.createMenuItem("Add...", 'A', this));
/*  196 */     KJUITools.addMenuItem(wTrackListMenu, KJUITools.createMenuItem("Import...", 'I', this));
/*  197 */     wTrackListMenu.addSeparator();
/*  198 */     KJUITools.addMenuItem(wTrackListMenu, KJUITools.createMenuItem("Remove...", 'R', this));
/*  199 */     KJUITools.addMenuItem(wTrackListMenu, KJUITools.createMenuItem("Rename...", 'N', this));
/*  200 */     wTrackListMenu.addSeparator();
/*  201 */     KJUITools.addMenuItem(wTrackListMenu, KJUITools.createMenuItem("Preferences...", 'P', this));
/*  202 */     wTrackListMenu.addSeparator();
/*      */ 
/*  204 */     JMenu wPickModeMenu = KJUITools.addMenu(wTrackListMenu, "Pick Mode", 'K');
/*      */ 
/*  206 */     this.trackPickModeSingle = ((JCheckBoxMenuItem)KJUITools.addMenuItem(wPickModeMenu, KJUITools.createCheckMenuItem("Single", 'S', this)));
/*  207 */     this.trackPickModeSequencial = ((JCheckBoxMenuItem)KJUITools.addMenuItem(wPickModeMenu, KJUITools.createCheckMenuItem("Sequencial", 'Q', this)));
/*  208 */     this.trackPickModeRandom = ((JCheckBoxMenuItem)KJUITools.addMenuItem(wPickModeMenu, KJUITools.createCheckMenuItem("Random", 'R', this)));
/*  209 */     this.trackPickModeProportional = ((JCheckBoxMenuItem)KJUITools.addMenuItem(wPickModeMenu, KJUITools.createCheckMenuItem("Proportional", 'P', this)));
/*      */ 
/*  211 */     wTrackListMenu.addSeparator();
/*      */ 
/*  213 */     KJUITools.addMenuItem(wTrackListMenu, KJUITools.createMenuItem("Close", 'C', this));
/*      */ 
/*  215 */     JMenu wTrackMenu = KJUITools.addMenu(wMenuBar, "Tracks", 'T');
/*      */ 
/*  217 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Add Files...", 'F', "ctrl F", this));
/*  218 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Add Folder...", 'L', "ctrl L", this));
/*  219 */     wTrackMenu.addSeparator();
/*      */ 
/*  221 */     JMenu wMoveMenu = KJUITools.addMenu(wTrackMenu, "Move", 'M');
/*      */ 
/*  223 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Top", 'T', "alt HOME", this));
/*  224 */     wMoveMenu.addSeparator();
/*  225 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Up 10", 'P', "alt PAGE_UP", this));
/*  226 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Up", 'U', "alt UP", this));
/*  227 */     wMoveMenu.addSeparator();
/*  228 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Down", 'D', "alt DOWN", this));
/*  229 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Down 10", 'W', "alt PAGE_DOWN", this));
/*  230 */     wMoveMenu.addSeparator();
/*  231 */     KJUITools.addMenuItem(wMoveMenu, KJUITools.createMenuItem("Bottom", 'B', "alt END", this));
/*      */ 
/*  233 */     wTrackMenu.addSeparator();
/*      */ 
/*  235 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Remove", 'R', "DELETE", this));
/*  236 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Remove All", 'A', "shift DELETE", this));
/*  237 */     wTrackMenu.addSeparator();
/*      */ 
/*  239 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Play Now", 'O', "ctrl O", this));
/*  240 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Play Next", 'X', "ctrl X", this));
/*  241 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Request", 'Q', "ctrl Q", this));
/*  242 */     wTrackMenu.addSeparator();
/*      */ 
/*  244 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Reset Played Tracks", 'R', this));
/*  245 */     wTrackMenu.addSeparator();
/*      */ 
/*  247 */     KJUITools.addMenuItem(wTrackMenu, KJUITools.createMenuItem("Properties...", 'P', "ctrl P", this));
/*      */ 
/*  249 */     JMenu wSearchMenu = KJUITools.addMenu(wMenuBar, "Search", 'S');
/*      */ 
/*  251 */     KJUITools.addMenuItem(wSearchMenu, KJUITools.createMenuItem("Find", 'F', "ctrl F3", this));
/*  252 */     KJUITools.addMenuItem(wSearchMenu, KJUITools.createMenuItem("Find Again", 'A', "F3", this));
/*  253 */     wSearchMenu.addSeparator();
/*  254 */     KJUITools.addMenuItem(wSearchMenu, KJUITools.createMenuItem("Find Playing", 'P', "P", this));
/*  255 */     KJUITools.addMenuItem(wSearchMenu, KJUITools.createMenuItem("Find Next Playing", 'N', "N", this));
/*      */ 
/*  266 */     JMenu wToolsMenu = KJUITools.addMenu(wMenuBar, "Tools", 'T');
/*      */ 
/*  268 */     KJUITools.addMenuItem(wToolsMenu, KJUITools.createMenuItem("Clean...", 'C', this));
/*      */ 
/*  270 */     JMenu wSortMenu = KJUITools.addMenu(wToolsMenu, "Sort", 'S');
/*  271 */     KJUITools.addMenuItem(wSortMenu, KJUITools.createMenuItem("Ascending", 'A', this));
/*  272 */     KJUITools.addMenuItem(wSortMenu, KJUITools.createMenuItem("Descending", 'D', this));
/*      */ 
/*  274 */     this.trackListPopupMenu = new JPopupMenu("Tracks");
/*      */ 
/*  276 */     KJUITools.addMenuItem(this.trackListPopupMenu, KJUITools.createMenuItem("Request", 'Q', this));
/*  277 */     this.trackListPopupMenu.addSeparator();
/*  278 */     KJUITools.addMenuItem(this.trackListPopupMenu, KJUITools.createMenuItem("Play Now", 'N', this));
/*  279 */     KJUITools.addMenuItem(this.trackListPopupMenu, KJUITools.createMenuItem("Play Next", 'X', this));
/*  280 */     this.trackListPopupMenu.addSeparator();
/*  281 */     KJUITools.addMenuItem(this.trackListPopupMenu, KJUITools.createMenuItem("Properties...", 'P', this));
/*      */   }
/*      */ 
/*      */   private void requestSelectedTracks()
/*      */   {
/*  287 */     int[] wSr = this.trackListTable.getSelectedRows();
/*      */ 
/*  289 */     KJTrack[] wReqestedTracks = new KJTrack[wSr.length];
/*      */ 
/*  291 */     for (int a = 0; a < wSr.length; a++) {
/*  292 */       wReqestedTracks[a] = ((KJTrack)this.trackListTable.getValueAt(wSr[a], 0));
/*      */     }
/*      */ 
/*  295 */     this.venue.getRequestList().addTracks(wReqestedTracks);
/*      */   }
/*      */ 
/*      */   private void selectActiveTrackList()
/*      */   {
/*  301 */     KJTrackList[] wAtl = this.venue.getActiveTrackLists();
/*      */ 
/*  303 */     if (wAtl.length > 0) {
/*  304 */       this.trackListSelector.setSelectedItem(wAtl[0]);
/*  305 */       this.trackListSelector.repaint();
/*  306 */       refreshTrackListActiveState();
/*      */     }
/*      */   }
/*      */ 
/*      */   private void selectTrackList(int pIndex)
/*      */   {
/*  313 */     if (this.trackListSelector.getItemCount() > 0)
/*      */     {
/*  315 */       if (pIndex >= this.trackListSelector.getItemCount())
/*  316 */         this.trackListSelector.setSelectedIndex(this.trackListSelector.getItemCount() - 1);
/*      */       else
/*  318 */         this.trackListSelector.setSelectedIndex(pIndex);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void setTrackPickMode(int pTrackPickMode)
/*      */   {
/*  327 */     switch (pTrackPickMode)
/*      */     {
/*      */     case 0:
/*  330 */       applyTrackPickMode(this.trackPickModeSingle, pTrackPickMode);
/*  331 */       break;
/*      */     case 1:
/*  334 */       applyTrackPickMode(this.trackPickModeSequencial, pTrackPickMode);
/*  335 */       break;
/*      */     case 2:
/*  338 */       applyTrackPickMode(this.trackPickModeRandom, pTrackPickMode);
/*  339 */       break;
/*      */     case 3:
/*  342 */       applyTrackPickMode(this.trackPickModeProportional, pTrackPickMode);
/*      */     }
/*      */   }
/*      */ 
/*      */   public void playerEvent(KJPlayerEvent pEvent)
/*      */   {
/*  350 */     refreshTrackList();
/*      */   }
/*      */ 
/*      */   private void prepareNextTrack()
/*      */   {
/*  355 */     int wSti = getSelectedTrackIndex();
/*      */ 
/*  357 */     if (wSti != -1)
/*      */     {
/*  359 */       KJTrackList wSpl = getSelectedTrackList();
/*      */ 
/*  361 */       wSpl.setCurrentTrackIndex(wSti);
/*  362 */       this.venue.clearPreparedTrack();
/*      */ 
/*  364 */       KJJukeBox.getPlayer().next(wSpl.getTrack(wSti));
/*      */     }
/*      */   }
/*      */ 
/*      */   public void venueChanged(KJVenue pVenue)
/*      */   {
/*  372 */     this.venue = pVenue;
/*      */ 
/*  374 */     refreshTrackLists();
/*      */ 
/*  376 */     selectActiveTrackList();
/*      */ 
/*  378 */     setTrackPickMode(this.venue.getTrackListPickMode());
/*      */ 
/*  380 */     this.venue.addListener(
/*  381 */       new KJTrackProviderChangeListener() {
/*      */       public void trackProviderChanged(KJTrackProviderChangeEvent pEvent) {
/*  383 */         KJEditTrackListsDialog.this.refreshTrackList();
/*      */       }
/*      */     });
/*      */   }
/*      */ 
/*      */   private boolean isFileNameExtension(File pFile, String pExtension)
/*      */   {
/*  392 */     String wFileName = pFile.getName();
/*      */ 
/*  394 */     int wEi = wFileName.lastIndexOf('.');
/*      */ 
/*  396 */     if (wEi != -1) {
/*  397 */       return wFileName.substring(wEi + 1).equalsIgnoreCase(pExtension);
/*      */     }
/*      */ 
/*  400 */     return false;
/*      */   }
/*      */ 
/*      */   private void sort(boolean pAscending)
/*      */   {
/*  406 */     int wSc = JOptionPane.showConfirmDialog(
/*  407 */       this, 
/*  408 */       "Are you sure you want to sort this track list?", 
/*  409 */       "Sort Track List", 
/*  410 */       0);
/*      */ 
/*  412 */     if (wSc == 0)
/*  413 */       getSelectedTrackList().sort(pAscending);
/*      */   }
/*      */ 
/*      */   public void addFolder(File pFolder, ArrayList pFiles, KJTrackList pTrackList)
/*      */   {
/*  427 */     File[] wFiles = pFolder.listFiles();
/*      */ 
/*  429 */     for (int a = 0; a < wFiles.length; a++)
/*      */     {
/*  431 */       File wFile = wFiles[a];
/*      */ 
/*  433 */       if (wFile.isDirectory())
/*  434 */         addFolder(wFile, pFiles, pTrackList);
/*  435 */       else if ((isFileNameExtension(wFile, "mp3")) || (isFileNameExtension(wFile, "mpS")))
/*  436 */         pFiles.add(new KJMP3Track(pTrackList, wFile, this.juke_box));
/*      */     }
/*      */   }
/*      */ 
/*      */   private void addAndSelectTracks(KJTrackList pTrackList, KJTrack[] pTracks)
/*      */   {
/*  445 */     pTrackList.addTracks(pTracks);
/*      */ 
/*  447 */     int wSi = pTrackList.size() - 1;
/*      */ 
/*  449 */     this.trackListTable.setRowSelectionInterval(wSi - (pTracks.length - 1), wSi);
/*  450 */     this.trackListTable.scrollRectToVisible(this.trackListTable.getCellRect(wSi, wSi, true));
/*      */   }
/*      */ 
/*      */   private void applyTrackPickMode(JCheckBoxMenuItem pMenuItem, int pTpm)
/*      */   {
/*  456 */     if (!this.ignoreStateChange)
/*      */     {
/*  458 */       this.ignoreStateChange = true;
/*      */ 
/*  460 */       this.trackPickModeSingle.setSelected(false);
/*  461 */       this.trackPickModeSequencial.setSelected(false);
/*  462 */       this.trackPickModeRandom.setSelected(false);
/*  463 */       this.trackPickModeProportional.setSelected(false);
/*      */ 
/*  465 */       pMenuItem.setSelected(true);
/*      */ 
/*  467 */       this.venue.setTrackListPickMode(pTpm, getSelectedTrackList());
/*      */ 
/*  469 */       this.ignoreStateChange = false;
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miTrackListsAdd(JMenuItem pMenuItem)
/*      */   {
/*  477 */     String wTrackListName = JOptionPane.showInputDialog(
/*  478 */       this, 
/*  479 */       "Please enter a name for this track list:", 
/*  480 */       "Add Track List", 
/*  481 */       -1);
/*      */ 
/*  483 */     if (wTrackListName != null)
/*      */     {
/*  485 */       this.venue.addTrackList(new KJTrackList(wTrackListName));
/*      */ 
/*  487 */       refreshTrackLists();
/*      */ 
/*  490 */       this.trackListSelector.setSelectedIndex(this.trackListSelector.getItemCount() - 1);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miTrackListsClose(JMenuItem pMenuItem)
/*      */   {
/*  497 */     setVisible(false);
/*      */   }
/*      */ 
/*      */   private void miTracksAddFiles(JMenuItem pMenuItem)
/*      */   {
/*  506 */     File[] wSf = KJUITools.createFileChooser(
/*  507 */       this, 
/*  508 */       "Add Files", 
/*  509 */       new KJUITools.FileFilterMP3andMPS());
/*      */ 
/*  523 */     addTrackToList(wSf);
/*      */   }
/*      */ 
/*      */   public void addTrackToList(File[] tracks)
/*      */   {
/*  535 */     KJTrackList wCurrentTrackList = getSelectedTrackList();
/*      */ 
/*  537 */     if ((wCurrentTrackList != null) && (tracks != null)) {
/*  538 */       KJTrack[] wTracks = new KJTrack[tracks.length];
/*      */ 
/*  540 */       for (int i = 0; i < tracks.length; i++) {
/*  541 */         wTracks[i] = new KJMP3Track(wCurrentTrackList, tracks[i], this.juke_box);
/*      */       }
/*      */ 
/*  544 */       addAndSelectTracks(wCurrentTrackList, wTracks);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miTracksAddFolder(JMenuItem pMenuItem)
/*      */   {
/*  551 */     KJTrackList wCurrentTrackList = getSelectedTrackList();
/*      */ 
/*  553 */     if (wCurrentTrackList != null)
/*      */     {
/*  555 */       File wFolder = KJUITools.createFolderChooser(
/*  556 */         this, 
/*  557 */         "Add Folder");
/*      */ 
/*  560 */       if (wFolder != null)
/*      */       {
/*  562 */         ArrayList wFiles = new ArrayList();
/*      */ 
/*  564 */         addFolder(wFolder, wFiles, wCurrentTrackList);
/*      */ 
/*  566 */         addAndSelectTracks(wCurrentTrackList, (KJTrack[])wFiles.toArray(new KJTrack[wFiles.size()]));
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miTracksRemove(JMenuItem pMenuItem)
/*      */   {
/*  575 */     getSelectedTrackList().removeTracks(this.trackListTable.getSelectedRows());
/*      */   }
/*      */ 
/*      */   private void miTracksRemoveAll(JMenuItem pMenuItem)
/*      */   {
/*  580 */     KJTrackList wTrackList = getSelectedTrackList();
/*      */ 
/*  582 */     int wRc = JOptionPane.showConfirmDialog(
/*  583 */       this, 
/*  584 */       "Remove all track from track list?", 
/*  585 */       "Remove All", 
/*  586 */       0);
/*      */ 
/*  588 */     if (wRc == 0)
/*  589 */       wTrackList.clear();
/*      */   }
/*      */ 
/*      */   private void miTracksRequest(JMenuItem pMenuItem)
/*      */   {
/*  595 */     requestSelectedTracks();
/*      */   }
/*      */ 
/*      */   private void miTrackListsImport(JMenuItem pMenuItem)
/*      */   {
/*  602 */     File[] wFiles = KJUITools.createFileChooser(
/*  603 */       this, 
/*  604 */       "Import", 
/*  605 */       new KJUITools.FileFilterWinampPlayList());
/*      */ 
/*  608 */     if (wFiles != null)
/*      */     {
/*  610 */       for (int a = 0; a < wFiles.length; a++)
/*      */       {
/*      */         try
/*      */         {
/*  616 */           this.venue.addTrackList(importWinampPlayList(wFiles[a]));
/*      */         }
/*      */         catch (IOException pEx)
/*      */         {
/*  620 */           JOptionPane.showMessageDialog(
/*  621 */             this, 
/*  622 */             pEx.toString(), 
/*  623 */             "Import Error!", 
/*  624 */             0);
/*      */         }
/*      */ 
/*      */       }
/*      */ 
/*  630 */       refreshTrackLists();
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miToolsClean(JMenuItem pMenuItem)
/*      */   {
/*  638 */     KJTrackList wTrackList = getSelectedTrackList();
/*      */ 
/*  640 */     new KJTrackListCleaner(this, this.venue, wTrackList);
/*      */ 
/*  642 */     refreshTrackList();
/*      */   }
/*      */ 
/*      */   private File getRoot(File pLocation, String pTestFile)
/*      */   {
/*  648 */     File wParent = pLocation;
/*      */ 
/*  652 */     while ((wParent = wParent.getParentFile()) != null)
/*      */     {
/*  656 */       if (new File(wParent, pTestFile).exists())
/*      */       {
/*      */         break;
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  664 */     return wParent;
/*      */   }
/*      */ 
/*      */   private KJTrackList importWinampPlayList(File pFile)
/*      */     throws IOException
/*      */   {
/*  670 */     KJTrackList wTrackList = new KJTrackList(pFile.getName());
/*      */ 
/*  672 */     File wRoot = null;
/*      */ 
/*  676 */     BufferedReader wBr = new BufferedReader(new FileReader(pFile));
/*      */ 
/*  678 */     String wHeader = wBr.readLine();
/*      */ 
/*  680 */     String wTitle = null;
/*      */ 
/*  682 */     while ((wTitle = wBr.readLine()) != null)
/*      */     {
/*  684 */       String wFile = wBr.readLine();
/*      */ 
/*  686 */       if (wRoot == null) {
/*  687 */         wRoot = getRoot(pFile, wFile);
/*      */       }
/*      */ 
/*  690 */       wTrackList.addTrack(new KJMP3Track(wTrackList, new File(wRoot, wFile), this.juke_box));
/*      */     }
/*      */ 
/*  694 */     return wTrackList;
/*      */   }
/*      */ 
/*      */   private void refreshTrackLists()
/*      */   {
/*  700 */     this.trackListSelectorModel.update();
/*      */   }
/*      */ 
/*      */   public void refreshTrackList()
/*      */   {
/*  711 */     ((KJListModel)this.trackListTable.getModel()).update();
/*      */   }
/*      */ 
/*      */   private void refreshTrackListActiveState()
/*      */   {
/*  716 */     KJTrackList wTrackList = getSelectedTrackList();
/*      */ 
/*  718 */     ((KJTrackListTableModel)this.trackListTable.getModel()).setTrackList(wTrackList);
/*      */ 
/*  720 */     this.ignoreStateChange = true;
/*      */ 
/*  722 */     if ((wTrackList != null) && (wTrackList.isActivatable()))
/*      */     {
/*  724 */       this.activeTrackList.setEnabled(true);
/*  725 */       this.activeTrackList.setSelected(wTrackList.isActive());
/*      */     }
/*      */     else {
/*  728 */       this.activeTrackList.setEnabled(false);
/*  729 */       this.activeTrackList.setSelected(false);
/*      */     }
/*      */ 
/*  732 */     this.ignoreStateChange = false;
/*      */   }
/*      */ 
/*      */   private void showSearchResults(KJVenue.SearchResults pSearchResults)
/*      */   {
/*  738 */     if (pSearchResults != null)
/*  739 */       showTrack(pSearchResults.trackListIndex, pSearchResults.trackIndex);
/*      */     else
/*  741 */       this.trackListTable.clearSelection();
/*      */   }
/*      */ 
/*      */   public void showTrack(KJTrack pTrack)
/*      */   {
/*  747 */     showSearchResults(this.venue.findTrack(pTrack));
/*      */   }
/*      */ 
/*      */   private void showTrack(int pTrackListIndex, int pTrackIndex)
/*      */   {
/*  753 */     this.trackListSelector.setSelectedIndex(pTrackListIndex);
/*      */ 
/*  756 */     EventQueue.invokeLater(
/*  757 */       new Runnable(pTrackIndex) {
/*      */       public void run() {
/*  759 */         KJEditTrackListsDialog.this.trackListTable.setRowSelectionInterval(this.val$pTrackIndex, this.val$pTrackIndex);
/*  760 */         KJEditTrackListsDialog.this.trackListTable.scrollRectToVisible(KJEditTrackListsDialog.this.trackListTable.getCellRect(this.val$pTrackIndex, this.val$pTrackIndex, true));
/*      */       }
/*      */     });
/*      */   }
/*      */ 
/*      */   private void showTrackProperties(JTable pTrackTable)
/*      */   {
/*  769 */     int[] wSrs = pTrackTable.getSelectedRows();
/*  770 */     KJTrack[] wSelectedTracks = new KJTrack[wSrs.length];
/*      */ 
/*  772 */     KJTrackList wSpl = getSelectedTrackList();
/*      */ 
/*  774 */     for (int a = 0; a < wSrs.length; a++) {
/*  775 */       wSelectedTracks[a] = wSpl.getTrack(wSrs[a]);
/*      */     }
/*      */ 
/*  778 */     new KJTrackProperties(this, wSelectedTracks);
/*      */   }
/*      */ 
/*      */   private void mcPickModeSingle(JCheckBoxMenuItem pMenuItem)
/*      */   {
/*  783 */     applyTrackPickMode(pMenuItem, 0);
/*      */   }
/*      */ 
/*      */   private void mcPickModeSequencial(JCheckBoxMenuItem pMenuItem) {
/*  787 */     applyTrackPickMode(pMenuItem, 1);
/*      */   }
/*      */ 
/*      */   private void mcPickModeRandom(JCheckBoxMenuItem pMenuItem) {
/*  791 */     applyTrackPickMode(pMenuItem, 2);
/*      */   }
/*      */ 
/*      */   private void mcPickModeProportional(JCheckBoxMenuItem pMenuItem) {
/*  795 */     applyTrackPickMode(pMenuItem, 3);
/*      */   }
/*      */ 
/*      */   private void miTrackListsPreferences(JMenuItem pMenuItem)
/*      */   {
/*  800 */     new KJTrackListPreferences(this, getSelectedTrackList());
/*      */ 
/*  806 */     refreshTrackListActiveState();
/*      */   }
/*      */ 
/*      */   private void miTrackListsRemove(JMenuItem pMenuItem)
/*      */   {
/*  812 */     KJTrackList wTrackList = getSelectedTrackList();
/*      */ 
/*  814 */     int wSi = this.trackListSelector.getSelectedIndex();
/*      */ 
/*  816 */     int wRc = JOptionPane.showConfirmDialog(
/*  817 */       this, 
/*  818 */       "Remove track list: '" + wTrackList.getName() + "'?", 
/*  819 */       "Remove Track List", 
/*  820 */       0);
/*      */ 
/*  822 */     if (wRc == 0)
/*      */     {
/*  824 */       this.venue.removePlayList(wTrackList);
/*      */ 
/*  826 */       refreshTrackLists();
/*      */ 
/*  828 */       selectTrackList(wSi);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miTrackListsRename(JMenuItem pMenuItem)
/*      */   {
/*  836 */     KJTrackList wTrackList = getSelectedTrackList();
/*      */ 
/*  838 */     String wTrackListName = JOptionPane.showInputDialog(
/*  839 */       this, 
/*  840 */       "Please enter another name for this track list:", 
/*  841 */       "Rename track List", 
/*  842 */       -1);
/*      */ 
/*  844 */     if (wTrackListName != null)
/*      */     {
/*  846 */       wTrackList.setName(wTrackListName);
/*      */ 
/*  848 */       refreshTrackLists();
/*      */     }
/*      */   }
/*      */ 
/*      */   private void mcTracksMarkPlayedTracks(JCheckBoxMenuItem pMenuItem)
/*      */   {
/*  855 */     KJJukeBox.getPlayer().setMarkPlayedTracks(pMenuItem.isSelected());
/*      */   }
/*      */ 
/*      */   private void miTracksResetPlayedTracks(JMenuItem pMenuItem)
/*      */   {
/*  860 */     KJTrackList wTrackList = getSelectedTrackList();
/*      */ 
/*  862 */     int wRc = JOptionPane.showConfirmDialog(
/*  863 */       this, 
/*  864 */       "Reset all played tracks in track list: '" + wTrackList.getName() + "'?", 
/*  865 */       "Reset Played Tracks", 
/*  866 */       0);
/*      */ 
/*  868 */     if (wRc == 0)
/*  869 */       wTrackList.resetPlayedTracks();
/*      */   }
/*      */ 
/*      */   private void miMoveTop(JMenuItem pMenuItem)
/*      */   {
/*  875 */     move(this.trackListTable.getSelectedRows(), -getSelectedTrackList().size());
/*      */   }
/*      */ 
/*      */   private void miMoveUp10(JMenuItem pMenuItem) {
/*  879 */     move(this.trackListTable.getSelectedRows(), -10);
/*      */   }
/*      */ 
/*      */   private void miMoveUp(JMenuItem pMenuItem) {
/*  883 */     move(this.trackListTable.getSelectedRows(), -1);
/*      */   }
/*      */ 
/*      */   private void miMoveDown(JMenuItem pMenuItem) {
/*  887 */     move(this.trackListTable.getSelectedRows(), 1);
/*      */   }
/*      */ 
/*      */   private void miMoveDown10(JMenuItem pMenuItem) {
/*  891 */     move(this.trackListTable.getSelectedRows(), 10);
/*      */   }
/*      */ 
/*      */   private void miMoveBottom(JMenuItem pMenuItem) {
/*  895 */     move(this.trackListTable.getSelectedRows(), getSelectedTrackList().size());
/*      */   }
/*      */ 
/*      */   private void miTracksPlayNow(JMenuItem pMenuItem)
/*      */   {
/*  900 */     prepareNextTrack();
/*      */ 
/*  902 */     KJJukeBox.getPlayer().next();
/*  903 */     KJJukeBox.getPlayer().play();
/*      */   }
/*      */ 
/*      */   private void miTracksPlayNext(JMenuItem pMenuItem)
/*      */   {
/*  908 */     prepareNextTrack();
/*      */   }
/*      */ 
/*      */   private void miTracksProperties(JMenuItem pMenuItem)
/*      */   {
/*  913 */     int[] wSrs = this.trackListTable.getSelectedRows();
/*  914 */     KJTrack[] wSelectedTracks = new KJTrack[wSrs.length];
/*      */ 
/*  916 */     KJTrackList wSpl = getSelectedTrackList();
/*      */ 
/*  918 */     for (int a = 0; a < wSrs.length; a++) {
/*  919 */       wSelectedTracks[a] = wSpl.getTrack(wSrs[a]);
/*      */     }
/*      */ 
/*  922 */     if (wSelectedTracks.length > 0)
/*  923 */       new KJTrackProperties(this, wSelectedTracks);
/*      */   }
/*      */ 
/*      */   private void miSearchFind(JMenuItem pItem)
/*      */   {
/*  932 */     String wSearchQuery = JOptionPane.showInputDialog(
/*  933 */       this, 
/*  934 */       "Enter part of a track name to find:", 
/*  935 */       this.venue.getSearchQuery());
/*      */ 
/*  937 */     if (wSearchQuery != null)
/*      */     {
/*  939 */       showSearchResults(
/*  940 */         this.venue.find(
/*  941 */         wSearchQuery, 
/*  942 */         this.trackListSelector.getSelectedIndex(), 
/*  943 */         this.trackListTable.getSelectedRow() + 1));
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miSearchFindAgain(JMenuItem pItem)
/*      */   {
/*  951 */     showSearchResults(
/*  952 */       this.venue.findNext(
/*  953 */       this.trackListSelector.getSelectedIndex(), 
/*  954 */       this.trackListTable.getSelectedRow() + 1));
/*      */   }
/*      */ 
/*      */   private void miSearchFindPlaying(JMenuItem pItem)
/*      */   {
/*  959 */     showSearchResults(
/*  960 */       this.venue.findTrack(KJJukeBox.getPlayer().getCurrentTrack()));
/*      */   }
/*      */ 
/*      */   private void miSearchFindNextPlaying(JMenuItem pItem) {
/*  964 */     showSearchResults(
/*  965 */       this.venue.findTrack(KJJukeBox.getNextTrack()));
/*      */   }
/*      */ 
/*      */   private void miSortAscending(JMenuItem pItem) {
/*  969 */     sort(true);
/*      */   }
/*      */ 
/*      */   private void miSortDescending(JMenuItem pItem) {
/*  973 */     sort(false);
/*      */   }
/*      */ 
/*      */   public void readXml(Element pSource)
/*      */   {
/* 1088 */     Element wBounds = KJXmlToolkit.find(pSource, "Bounds");
/*      */ 
/* 1090 */     if (wBounds != null)
/*      */     {
/* 1092 */       setBounds(
/* 1093 */         Integer.parseInt(wBounds.getAttribute("x")), 
/* 1094 */         Integer.parseInt(wBounds.getAttribute("y")), 
/* 1095 */         Integer.parseInt(wBounds.getAttribute("width")), 
/* 1096 */         Integer.parseInt(wBounds.getAttribute("height")));
/*      */     }
/*      */ 
/* 1100 */     setVisible(KJXmlToolkit.get(pSource, "Visible", true));
/*      */   }
/*      */ 
/*      */   public void writeXml(Element pTarget)
/*      */   {
/* 1109 */     Element wBounds = KJXmlToolkit.add(pTarget, "Bounds");
/*      */ 
/* 1111 */     wBounds.setAttribute("x", String.valueOf(getBounds().x));
/* 1112 */     wBounds.setAttribute("y", String.valueOf(getBounds().y));
/* 1113 */     wBounds.setAttribute("width", String.valueOf(getBounds().width));
/* 1114 */     wBounds.setAttribute("height", String.valueOf(getBounds().height));
/*      */ 
/* 1116 */     KJXmlToolkit.set(pTarget, "Visible", isVisible());
/*      */   }
/*      */ 
/*      */   private class TrackListActivationListener
/*      */     implements ItemListener
/*      */   {
/*      */     private TrackListActivationListener()
/*      */     {
/*      */     }
/*      */ 
/*      */     public void itemStateChanged(ItemEvent pEvent)
/*      */     {
/*  980 */       if (!KJEditTrackListsDialog.this.ignoreStateChange)
/*      */       {
/*  982 */         KJTrackList wSelectedTrackList = (KJTrackList)KJEditTrackListsDialog.this.trackListSelector.getSelectedItem();
/*      */ 
/*  984 */         wSelectedTrackList.setActive(!wSelectedTrackList.isActive());
/*      */ 
/*  986 */         KJEditTrackListsDialog.this.activeTrackList.setSelected(wSelectedTrackList.isActive());
/*      */ 
/*  988 */         KJEditTrackListsDialog.this.refreshTrackLists();
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private class TrackListChangeListener implements ItemListener
/*      */   {
/*      */     private TrackListChangeListener()
/*      */     {
/*      */     }
/*      */ 
/*      */     public void itemStateChanged(ItemEvent pEvent) {
/* 1000 */       KJEditTrackListsDialog.this.refreshTrackListActiveState();
/*      */     }
/*      */   }
/*      */   private class TrackListModel implements ComboBoxModel {
/* 1008 */     private ArrayList listeners = new ArrayList();
/*      */     private Object selected;
/*      */ 
/*      */     private TrackListModel() {
/*      */     }
/*      */     public Object getSelectedItem() {
/* 1012 */       return this.selected;
/*      */     }
/*      */ 
/*      */     public void setSelectedItem(Object pItem) {
/* 1016 */       this.selected = pItem;
/*      */     }
/*      */ 
/*      */     public int getSize()
/*      */     {
/* 1021 */       if (KJEditTrackListsDialog.this.venue != null) {
/* 1022 */         return KJEditTrackListsDialog.this.venue.getTrackListCount();
/*      */       }
/* 1024 */       return 0;
/*      */     }
/*      */ 
/*      */     public Object getElementAt(int pIndex)
/*      */     {
/* 1030 */       return KJEditTrackListsDialog.this.venue.getTrackList(pIndex);
/*      */     }
/*      */ 
/*      */     public void addListDataListener(ListDataListener pListener) {
/* 1034 */       this.listeners.add(pListener);
/*      */     }
/*      */ 
/*      */     public void removeListDataListener(ListDataListener pListener) {
/* 1038 */       this.listeners.remove(pListener);
/*      */     }
/*      */ 
/*      */     public void update()
/*      */     {
/* 1043 */       ListDataEvent wEvent = new ListDataEvent(
/* 1044 */         this, 
/* 1045 */         0, 
/* 1046 */         0, 
/* 1047 */         getSize());
/*      */ 
/* 1049 */       for (int a = this.listeners.size() - 1; a >= 0; a--)
/* 1050 */         ((ListDataListener)this.listeners.get(a)).contentsChanged(wEvent);
/*      */     }
/*      */   }
/*      */ 
/*      */   private class TrackListMouseListener extends MouseAdapter
/*      */   {
/*      */     private TrackListMouseListener()
/*      */     {
/*      */     }
/*      */ 
/*      */     public void mouseClicked(MouseEvent pEvent) {
/* 1061 */       if ((pEvent.getButton() == 1) && (pEvent.getClickCount() == 2))
/*      */       {
/* 1063 */         KJEditTrackListsDialog.this.requestSelectedTracks();
/*      */       }
/* 1065 */       else if (pEvent.getButton() == 3)
/*      */       {
/* 1069 */         int wRap = KJEditTrackListsDialog.this.trackListTable.rowAtPoint(pEvent.getPoint());
/*      */ 
/* 1071 */         if (!KJEditTrackListsDialog.this.trackListTable.isRowSelected(wRap)) {
/* 1072 */           KJEditTrackListsDialog.this.trackListTable.setRowSelectionInterval(wRap, wRap);
/*      */         }
/*      */ 
/* 1075 */         KJEditTrackListsDialog.this.trackListPopupMenu.show(KJEditTrackListsDialog.this.trackListTable, pEvent.getX(), pEvent.getY());
/*      */       }
/*      */     }
/*      */   }
/*      */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJEditTrackListsDialog
 * JD-Core Version:    0.6.0
 */