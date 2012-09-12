/*     */ package kj.storeclient.ui;
/*     */ 
/*     */ import constants.CommunicationConstants;
/*     */ import java.awt.BorderLayout;
/*     */ import java.awt.Container;
/*     */ import java.awt.Point;
/*     */ import java.awt.Rectangle;
/*     */ import java.awt.event.ActionEvent;
/*     */ import java.awt.event.ActionListener;
/*     */ import java.io.IOException;
/*     */ import java.net.Socket;
/*     */ import javax.swing.BoxLayout;
/*     */ import javax.swing.JButton;
/*     */ import javax.swing.JDialog;
/*     */ import javax.swing.JOptionPane;
/*     */ import javax.swing.JPanel;
/*     */ import javax.swing.JTable;
/*     */ import kj.storeclient.KJStoreClient;
/*     */ import kj.storeclient.StoreTrack;
/*     */ import kj.ui.KJEditTrackListsDialog;
/*     */ import kj.ui.KJJukeBox;
/*     */ import kj.ui.KJUITools;
/*     */ import kj.xml.KJXmlSerializable;
/*     */ import kj.xml.KJXmlToolkit;
/*     */ import org.w3c.dom.Element;
/*     */ 
/*     */ public class KJStoreClientUI extends JDialog
/*     */   implements KJXmlSerializable, CommunicationConstants
/*     */ {
/*  58 */   private String CONNECT_TEXT = "Connect";
/*     */ 
/*  60 */   private String DISCONNECT_TEXT = "Disconnect";
/*     */   private JButton updateButton_;
/*     */   private JButton downloadButton_;
/*     */   private JTable trackListTable_;
/*     */   private KJStoreClientUITrackListTableModel trackListModel_;
/*     */   private KJStoreClient client_;
/*     */   private KJEditTrackListsDialog trackList_;
/*     */   private KJJukeBox juke_box;
/*     */ 
/*     */   private KJStoreClientUI()
/*     */   {
/*     */   }
/*     */ 
/*     */   public KJStoreClientUI(KJJukeBox parentFrame, KJEditTrackListsDialog trackList, Socket socket)
/*     */   {
/*  81 */     super(parentFrame);
/*  82 */     this.juke_box = parentFrame;
/*     */ 
/*  84 */     this.trackList_ = trackList;
/*  85 */     this.client_ = new KJStoreClient(this, socket, parentFrame);
/*     */ 
/*  87 */     prepareGUI();
/*     */   }
/*     */ 
/*     */   private void prepareGUI()
/*     */   {
/*  96 */     setTitle("KJ Store");
/*  97 */     setSize(480, 650);
/*     */ 
/*  99 */     setDefaultCloseOperation(1);
/*     */ 
/* 101 */     Point p = KJUITools.calculateCenterScreenPosition(getSize());
/* 102 */     setLocation(p.x + 600, p.y - 150);
/*     */ 
/* 104 */     getContentPane().setLayout(new BorderLayout());
/*     */ 
/* 107 */     JPanel wDisplayPanel = new JPanel();
/* 108 */     getContentPane().add(wDisplayPanel, "Center");
/*     */ 
/* 110 */     wDisplayPanel.setLayout(
/* 111 */       new BoxLayout(wDisplayPanel, 
/* 111 */       3));
/*     */ 
/* 113 */     JPanel trackPanel = new JPanel(new BorderLayout());
/* 114 */     wDisplayPanel.add(trackPanel, "Center");
/* 115 */     trackPanel.setBorder(
/* 116 */       KJUITools.createTitleBorder("Tracks Available for Download"));
/*     */ 
/* 118 */     JPanel buttonPanel = new JPanel();
/* 119 */     wDisplayPanel.add(buttonPanel);
/* 120 */     buttonPanel.setLayout(new BoxLayout(buttonPanel, 2));
/*     */ 
/* 124 */     this.updateButton_ = new JButton(); this.updateButton_.setText("Update");
/* 125 */     this.updateButton_.setVisible(true); this.updateButton_.addActionListener(new UpdateButtonAction(null));
/*     */ 
/* 128 */     this.downloadButton_ = new JButton("Download");
/* 129 */     this.downloadButton_.setVisible(true);
/* 130 */     this.downloadButton_.addActionListener(new DownloadButtonAction(null));
/*     */ 
/* 132 */     buttonPanel.add(this.updateButton_);
/* 133 */     buttonPanel.add(this.downloadButton_);
/*     */ 
/* 136 */     this.trackListTable_ = createTrackListTable();
/* 137 */     trackPanel.add(KJUITools.createScrollPane(this.trackListTable_), 
/* 138 */       "Center");
/*     */ 
/* 140 */     refreshTrackList();
/*     */   }
/*     */ 
/*     */   private JTable createTrackListTable()
/*     */   {
/* 148 */     JTable table = new JTable();
/* 149 */     this.trackListModel_ = new KJStoreClientUITrackListTableModel();
/* 150 */     this.trackListModel_.setTable(this.client_.getTracks());
/* 151 */     table.setModel(this.trackListModel_);
/* 152 */     table.setVisible(true);
/* 153 */     table.setTableHeader(null);
/* 154 */     table.setShowGrid(false);
/* 155 */     table.setSelectionMode(0);
/*     */ 
/* 157 */     return table;
/*     */   }
/*     */ 
/*     */   private void refreshTrackList()
/*     */   {
/*     */     try
/*     */     {
/* 166 */       this.client_.refreshSongListFromServer();
/*     */     }
/*     */     catch (IOException e) {
/* 169 */       e.printStackTrace();
/*     */     }
/*     */   }
/*     */ 
/*     */   public void readXml(Element pSource)
/*     */   {
/* 179 */     Element wBounds = KJXmlToolkit.find(pSource, "Bounds");
/*     */ 
/* 181 */     if (wBounds != null)
/*     */     {
/* 183 */       setBounds(Integer.parseInt(wBounds.getAttribute("x")), 
/* 184 */         Integer.parseInt(wBounds.getAttribute("y")), 
/* 185 */         Integer.parseInt(wBounds.getAttribute("width")), 
/* 186 */         Integer.parseInt(wBounds.getAttribute("height")));
/*     */     }
/*     */ 
/* 190 */     setVisible(KJXmlToolkit.get(pSource, "Visible", true));
/*     */   }
/*     */ 
/*     */   public void writeXml(Element pTarget)
/*     */   {
/* 199 */     Element wBounds = KJXmlToolkit.add(pTarget, "Bounds");
/*     */ 
/* 201 */     wBounds.setAttribute("x", String.valueOf(getBounds().x));
/* 202 */     wBounds.setAttribute("y", String.valueOf(getBounds().y));
/* 203 */     wBounds.setAttribute("width", String.valueOf(getBounds().width));
/* 204 */     wBounds.setAttribute("height", String.valueOf(getBounds().height));
/*     */ 
/* 206 */     KJXmlToolkit.set(pTarget, "Visible", isVisible());
/*     */   }
/*     */ 
/*     */   public boolean trackExists(String name)
/*     */   {
/* 260 */     int opt = JOptionPane.showConfirmDialog(this, 
/* 261 */       "The file you want to download already exists.\nWould you like to overwrite it?", 
/* 262 */       "Overwrite", 
/* 263 */       0);
/*     */ 
/* 265 */     return opt == 0;
/*     */   }
/*     */ 
/*     */   private class UpdateButtonAction
/*     */     implements ActionListener
/*     */   {
/*     */     private UpdateButtonAction()
/*     */     {
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent event)
/*     */     {
/* 218 */       KJStoreClientUI.this.refreshTrackList();
/* 219 */       KJStoreClientUI.this.trackListModel_.fireTableDataChanged();
/*     */ 
/* 221 */       KJStoreClientUI.this.downloadButton_.setEnabled(true);
/*     */     }
/*     */   }
/*     */ 
/*     */   private class DownloadButtonAction implements ActionListener
/*     */   {
/*     */     private DownloadButtonAction()
/*     */     {
/*     */     }
/*     */ 
/*     */     public void actionPerformed(ActionEvent event)
/*     */     {
/* 233 */       int[] rows = KJStoreClientUI.this.trackListTable_.getSelectedRows();
/* 234 */       String path = null;
/*     */ 
/* 236 */       for (int i = 0; i < rows.length; i++)
/* 237 */         if ((rows[i] >= 0) && (rows[i] < KJStoreClientUI.this.trackListTable_.getRowCount())) {
/* 238 */           StoreTrack track = (StoreTrack)KJStoreClientUI.this.trackListModel_.getValueAt(rows[i], 0);
/*     */ 
/* 241 */           path = KJStoreClientUI.this.client_.downloadTrack(track);
/*     */         }
/*     */     }
/*     */   }
/*     */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.storeclient.ui.KJStoreClientUI
 * JD-Core Version:    0.6.0
 */