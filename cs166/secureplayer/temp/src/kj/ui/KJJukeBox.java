/*      */ package kj.ui;
/*      */ 
/*      */ import constants.CommunicationConstants;
/*      */ import java.awt.Color;
/*      */ import java.awt.Container;
/*      */ import java.awt.Dimension;
/*      */ import java.awt.EventQueue;
/*      */ import java.awt.Font;
/*      */ import java.awt.Rectangle;
/*      */ import java.awt.Toolkit;
/*      */ import java.awt.event.ComponentAdapter;
/*      */ import java.awt.event.ComponentEvent;
/*      */ import java.awt.event.WindowAdapter;
/*      */ import java.awt.event.WindowEvent;
/*      */ import java.io.BufferedReader;
/*      */ import java.io.BufferedWriter;
/*      */ import java.io.File;
/*      */ import java.io.FileInputStream;
/*      */ import java.io.FileOutputStream;
/*      */ import java.io.IOException;
/*      */ import java.io.InputStreamReader;
/*      */ import java.io.OutputStreamWriter;
/*      */ import java.io.PrintStream;
/*      */ import java.net.Socket;
/*      */ import java.text.SimpleDateFormat;
/*      */ import java.util.ArrayList;
/*      */ import java.util.Date;
/*      */ import javax.swing.BorderFactory;
/*      */ import javax.swing.JButton;
/*      */ import javax.swing.JCheckBoxMenuItem;
/*      */ import javax.swing.JFrame;
/*      */ import javax.swing.JLabel;
/*      */ import javax.swing.JMenu;
/*      */ import javax.swing.JMenuBar;
/*      */ import javax.swing.JMenuItem;
/*      */ import javax.swing.JOptionPane;
/*      */ import javax.swing.JPanel;
/*      */ import javax.swing.JSlider;
/*      */ import javax.swing.UIManager;
/*      */ import javax.swing.event.ChangeEvent;
/*      */ import javax.swing.event.ChangeListener;
/*      */ import javax.swing.filechooser.FileFilter;
/*      */ import kj.audio.KJAudioDataProvider;
/*      */ import kj.player.KJPlayer;
/*      */ import kj.player.KJPlayerEvent;
/*      */ import kj.player.KJPlayerListener;
/*      */ import kj.storeclient.ui.KJStoreClientUI;
/*      */ import kj.track.KJTrack;
/*      */ import kj.track.KJTrackList;
/*      */ import kj.track.KJTrackProviderChangeEvent;
/*      */ import kj.track.KJTrackProviderChangeListener;
/*      */ import kj.venue.KJVenue;
/*      */ import kj.venue.KJVenueChangeListener;
/*      */ import kj.xml.KJXmlSerializable;
/*      */ import kj.xml.KJXmlToolkit;
/*      */ import org.w3c.dom.Document;
/*      */ import org.w3c.dom.Element;
/*      */ 
/*      */ public class KJJukeBox extends JFrame
/*      */   implements KJTrackProviderChangeListener, KJXmlSerializable, CommunicationConstants
/*      */ {
/*      */   KJStoreClientUI storeClient_;
/*      */   public static final String VERSION_STRING = "v0.45";
/*      */   public static final String VERSION_DATE = "August 2005";
/*   86 */   private static final SimpleDateFormat tf = new SimpleDateFormat("mm:ss");
/*      */   private static KJEditTrackListsDialog trackListEditor;
/*      */   private static KJRequestListDialog requestList;
/*      */   private static KJDSPDialog dsp;
/*      */   private static KJHelpDocumentation documentation;
/*      */   private static KJPlayer player;
/*      */   private static KJVenue venue;
/*      */   private static File venueLocation;
/*  102 */   private static ArrayList venueChangeListeners = new ArrayList();
/*      */ 
/*  104 */   private Object displayLock = new Object();
/*      */   private JLabel labelSongTitle;
/*      */   private JLabel labelNextTrack;
/*      */   private JLabel labelSongPosition;
/*      */   private JSlider positionSlider;
/*      */   private boolean positionIsAdjusting;
/*      */   private JCheckBoxMenuItem pauseCheckMenuItem;
/*  118 */   private boolean pauseIsAdjusting = false;
/*      */   private JCheckBoxMenuItem repeatCheckMenuItem;
/*      */   private JCheckBoxMenuItem fadeWhenNavigatingCheckMenuItem;
/*      */   private JSlider volumeSlider;
/*      */   private JCheckBoxMenuItem microphonePriorityCheckMenuItem;
/*      */   private JCheckBoxMenuItem muteCheckMenuItem;
/*  130 */   private boolean volumeIsAdjusting = false;
/*      */   private JCheckBoxMenuItem markPlayedTracks;
/*      */   private String username;
/*      */   private String password;
/*      */   private Socket server_;
/*      */   private BufferedReader serverReader_;
/*      */   private BufferedWriter serverWriter_;
/*      */ 
/*      */   public KJJukeBox()
/*      */     throws Exception
/*      */   {
/*  148 */     initialize();
/*      */   }
/*      */ 
/*      */   public static void addVenueChangeListener(KJVenueChangeListener pListener)
/*      */   {
/*  154 */     venueChangeListeners.add(pListener);
/*      */ 
/*  156 */     if (venue != null)
/*  157 */       pListener.venueChanged(venue);
/*      */   }
/*      */ 
/*      */   private void initialize()
/*      */   {
/*  164 */     addWindowListener(new WindowAdapter() {
/*      */       public void windowClosing(WindowEvent event) {
/*  166 */         KJJukeBox.this.disconnect();
/*      */       }
/*      */     });
/*  170 */     checkAccount();
/*  171 */     createPlayer();
/*      */ 
/*  173 */     minimizeThreadPriorities();
/*      */ 
/*  175 */     prepareGUI();
/*      */ 
/*  177 */     loadCurrentState();
/*      */ 
/*  179 */     startDisplayThread();
/*      */   }
/*      */ 
/*      */   public void connectToServer(String host)
/*      */   {
/*      */     try
/*      */     {
/*  186 */       this.server_ = new Socket(host, 8000);
/*      */     } catch (IOException ioe) {
/*  188 */       System.err.println("unknown host: \"localhost\"");
/*  189 */       ioe.printStackTrace();
/*  190 */       System.exit(-1);
/*      */     }String response;
/*      */     try {
/*  194 */       this.serverReader_ = 
/*  195 */         new BufferedReader(new InputStreamReader(this.server_.getInputStream()));
/*  196 */       this.serverWriter_ = 
/*  197 */         new BufferedWriter(new OutputStreamWriter(this.server_.getOutputStream()));
/*      */ 
/*  200 */       response = this.serverReader_.readLine();
/*      */     }
/*      */     catch (Exception localException)
/*      */     {
/*      */     }
/*      */   }
/*      */ 
/*      */   public Socket getSocket() {
/*  208 */     return this.server_;
/*      */   }
/*      */ 
/*      */   private void checkAccount()
/*      */   {
/*  215 */     Authenticator auth = new Authenticator(this, 
/*  216 */       Thread.currentThread());
/*  217 */     while (!auth.isReady()) {
/*  218 */       Thread.currentThread().setPriority(1);
/*      */       try {
/*  220 */         Thread.currentThread(); Thread.sleep(1000L);
/*      */       }
/*      */       catch (InterruptedException localInterruptedException)
/*      */       {
/*      */       }
/*      */     }
/*  226 */     this.username = auth.getUser();
/*  227 */     this.password = auth.getPass();
/*      */   }
/*      */ 
/*      */   public String getUsername()
/*      */   {
/*  232 */     return this.username;
/*      */   }
/*      */ 
/*      */   public String getPassword()
/*      */   {
/*  237 */     return this.password;
/*      */   }
/*      */ 
/*      */   public void disconnect()
/*      */   {
/*  244 */     if (this.server_ != null)
/*      */       try
/*      */       {
/*  247 */         this.serverWriter_.write("CLOSE_CONNECTION\n");
/*  248 */         this.serverWriter_.flush();
/*      */ 
/*  252 */         this.serverReader_.close();
/*  253 */         this.serverWriter_.close();
/*  254 */         this.server_.close();
/*      */ 
/*  256 */         this.serverReader_ = null;
/*  257 */         this.serverWriter_ = null;
/*  258 */         this.server_ = null;
/*      */       } catch (IOException ioe) {
/*  260 */         System.err.println("Error disconnecting from server");
/*  261 */         ioe.printStackTrace();
/*      */       }
/*      */   }
/*      */ 
/*      */   public void addSong(File trackFile)
/*      */   {
/*  267 */     File[] f = new File[1];
/*  268 */     f[0] = trackFile;
/*  269 */     trackListEditor.addTrackToList(f);
/*      */   }
/*      */ 
/*      */   private void createPlayer()
/*      */   {
/*  276 */     player = new KJPlayer();
/*      */ 
/*  278 */     player.addListener(new PlayerListener(null));
/*      */   }
/*      */ 
/*      */   private void minimizeThreadPriorities()
/*      */   {
/*  289 */     ThreadGroup wMtg = Thread.currentThread().getThreadGroup();
/*      */ 
/*  291 */     Thread[] wThreads = new Thread[wMtg.activeCount()];
/*      */ 
/*  293 */     wMtg.enumerate(wThreads, true);
/*      */ 
/*  295 */     for (int a = 0; a < wThreads.length; a++) {
/*  296 */       if (wThreads[a] != null) {
/*  297 */         wThreads[a].setPriority(1);
/*      */       }
/*      */     }
/*      */ 
/*      */     try
/*      */     {
/*  303 */       EventQueue.invokeAndWait(new Runnable() {
/*      */         public void run() {
/*  305 */           Thread.currentThread().setPriority(1);
/*      */         }
/*      */       });
/*      */     }
/*      */     catch (Exception localException)
/*      */     {
/*      */     }
/*      */   }
/*      */ 
/*      */   private void updateJukeBox() {
/*  316 */     this.fadeWhenNavigatingCheckMenuItem.setSelected(
/*  317 */       venue.isNavigationCrossFadingEnabled());
/*      */ 
/*  319 */     this.volumeSlider.setValue((int)venue.getGain());
/*      */   }
/*      */ 
/*      */   private void updatePlayerSettings()
/*      */   {
/*  325 */     player.setGain(venue.getGain());
/*      */ 
/*  327 */     player.setFadeWhenNavigating(venue.isNavigationCrossFadingEnabled());
/*  328 */     player.setNavigationCrossFader(venue.getNavigationCrossFader());
/*  329 */     player.setVolumeMonitoringEnabled(venue.isVolumeMonitoringEnabled());
/*  330 */     player.setVolumeMonitoringDelay(venue.getVolumeMonitoringDelay());
/*      */   }
/*      */ 
/*      */   private void updateVenue(KJVenue pVenue)
/*      */   {
/*  336 */     if (venue != null) {
/*  337 */       venue.removeListener(this);
/*      */     }
/*      */ 
/*  340 */     if (pVenue != null)
/*      */     {
/*  342 */       venue = pVenue;
/*      */ 
/*  344 */       player.setTrackProvider(venue);
/*      */ 
/*  346 */       for (int a = 0; a < venueChangeListeners.size(); a++) {
/*  347 */         ((KJVenueChangeListener)venueChangeListeners.get(a))
/*  348 */           .venueChanged(venue);
/*      */       }
/*      */ 
/*  351 */       venue.addListener(this);
/*      */ 
/*  353 */       showNextTrack();
/*      */ 
/*  355 */       updateJukeBox();
/*  356 */       updatePlayerSettings();
/*      */     }
/*      */   }
/*      */ 
/*      */   public void trackProviderChanged(KJTrackProviderChangeEvent pEvent)
/*      */   {
/*  363 */     showNextTrack();
/*      */   }
/*      */ 
/*      */   private void exitApplication()
/*      */   {
/*  368 */     if (player != null) {
/*  369 */       player.stop();
/*      */     }
/*      */ 
/*  373 */     disconnect();
/*      */ 
/*  375 */     dispose();
/*      */ 
/*  377 */     System.exit(0);
/*      */   }
/*      */ 
/*      */   public static KJTrack getNextTrack()
/*      */   {
/*  383 */     if (player.getNextTrack() != null) {
/*  384 */       return player.getNextTrack();
/*      */     }
/*  386 */     return venue.getPreparedTrack();
/*      */   }
/*      */ 
/*      */   public static KJPlayer getPlayer()
/*      */   {
/*  392 */     return player;
/*      */   }
/*      */ 
/*      */   private void startDisplayThread()
/*      */   {
/*  397 */     new Thread()
/*      */     {
/*      */       public void run()
/*      */       {
/*      */         while (true)
/*      */         {
/*  403 */           synchronized (KJJukeBox.this.displayLock)
/*      */           {
/*      */             try
/*      */             {
/*  407 */               if ((KJJukeBox.player == null) || (KJJukeBox.player.isStopped()))
/*      */                 continue;
/*  409 */               long wPbp = KJJukeBox.player.getCurrentPosition() / 1000L * 1000L;
/*  410 */               long wPbd = KJJukeBox.player.getCurrentTrack().duration();
/*      */ 
/*  412 */               KJJukeBox.this.labelSongPosition.setText(KJJukeBox.tf
/*  413 */                 .format(new Date(wPbp)) + 
/*  414 */                 " / " + 
/*  415 */                 KJJukeBox.tf.format(new Date(wPbd)) + 
/*  416 */                 " [-" + 
/*  417 */                 KJJukeBox.tf.format(new Date(wPbd - wPbp)) + 
/*  418 */                 "]");
/*      */ 
/*  420 */               if (KJJukeBox.this.positionSlider.getValueIsAdjusting())
/*      */                 continue;
/*  422 */               KJJukeBox.this.positionIsAdjusting = true;
/*  423 */               KJJukeBox.this.positionSlider.setValue(
/*  425 */                 (int)KJJukeBox.player.getCurrentAudioDataProvider()
/*  425 */                 .position());
/*  426 */               KJJukeBox.this.positionIsAdjusting = false; continue;
/*      */ 
/*  432 */               KJJukeBox.this.labelSongPosition
/*  433 */                 .setText("-:-- / 0:00 [-0:00]");
/*      */             }
/*      */             catch (Exception localException)
/*      */             {
/*      */             }
/*      */ 
/*      */           }
/*      */ 
/*      */           try
/*      */           {
/*  445 */             Thread.sleep(100L);
/*      */           }
/*      */           catch (InterruptedException e) {
/*  448 */             e.printStackTrace();
/*      */           }
/*      */         }
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  455 */     .start();
/*      */   }
/*      */ 
/*      */   private File getRoot(File pLocation)
/*      */   {
/*  461 */     File wParent = pLocation;
/*  462 */     File wLastParent = null;
/*      */ 
/*  464 */     while ((wParent = wParent.getParentFile()) != null) {
/*  465 */       wLastParent = wParent;
/*      */     }
/*      */ 
/*  470 */     return wLastParent;
/*      */   }
/*      */ 
/*      */   public static KJEditTrackListsDialog getPlayListEditorDialog()
/*      */   {
/*  477 */     return trackListEditor;
/*      */   }
/*      */ 
/*      */   public static KJRequestListDialog getRequestListDialog() {
/*  481 */     return requestList;
/*      */   }
/*      */ 
/*      */   public static KJDSPDialog getDSPDialog() {
/*  485 */     return dsp;
/*      */   }
/*      */ 
/*      */   private void prepareGUI()
/*      */   {
/*      */     try
/*      */     {
/*  498 */       UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
/*      */     } catch (Exception pEx) {
/*  500 */       pEx.printStackTrace();
/*      */     }
/*      */ 
/*  503 */     setTitle("KJ= v0.45");
/*  504 */     setSize(380, 200);
/*      */ 
/*  507 */     Dimension wSs = Toolkit.getDefaultToolkit().getScreenSize();
/*      */ 
/*  509 */     setLocation(wSs.width - getSize().width, 
/*  510 */       wSs.height - getSize().height - 32);
/*      */ 
/*  513 */     addWindowListener(new WindowAdapter() {
/*      */       public void windowClosing(WindowEvent pEvent) {
/*  515 */         KJJukeBox.this.exitApplication();
/*      */       }
/*      */     });
/*  519 */     getContentPane().setLayout(
/*  520 */       new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*  521 */     getContentPane().setBackground(Color.black);
/*      */ 
/*  523 */     JPanel wDisplay = new JPanel(new KJFlowLayout());
/*  524 */     getContentPane().add(wDisplay);
/*      */ 
/*  526 */     wDisplay.setBorder(BorderFactory.createCompoundBorder(
/*  527 */       BorderFactory.createLoweredBevelBorder(), 
/*  528 */       BorderFactory.createMatteBorder(8, 
/*  528 */       8, 8, 8, Color.black)));
/*      */ 
/*  530 */     wDisplay.setBackground(Color.black);
/*      */ 
/*  532 */     this.labelSongTitle = new JLabel("[No Track] <Stopped>");
/*  533 */     this.labelSongTitle.setPreferredSize(new Dimension(0, 20));
/*      */ 
/*  535 */     wDisplay.add(this.labelSongTitle);
/*      */ 
/*  537 */     this.labelSongTitle.setForeground(Color.red);
/*  538 */     this.labelSongTitle.setBackground(Color.black);
/*  539 */     this.labelSongTitle.setFont(this.labelSongTitle.getFont()
/*  540 */       .deriveFont(1, 
/*  540 */       12.0F));
/*  541 */     this.labelSongTitle.setOpaque(true);
/*      */ 
/*  543 */     this.labelNextTrack = new JLabel("Next: N/A");
/*  544 */     this.labelNextTrack.setPreferredSize(new Dimension(0, 20));
/*      */ 
/*  546 */     wDisplay.add(this.labelNextTrack);
/*      */ 
/*  548 */     this.labelNextTrack.setForeground(Color.orange);
/*  549 */     this.labelNextTrack.setBackground(Color.black);
/*  550 */     this.labelNextTrack.setOpaque(true);
/*      */ 
/*  552 */     this.labelSongPosition = new JLabel();
/*  553 */     wDisplay.add(this.labelSongPosition);
/*      */ 
/*  555 */     this.labelSongPosition.setForeground(new Color(128, 192, 255));
/*  556 */     this.labelSongPosition.setBackground(Color.black);
/*  557 */     this.labelSongPosition
/*  558 */       .setFont(this.labelSongPosition.getFont().deriveFont(24.0F));
/*  559 */     this.labelSongPosition.setOpaque(true);
/*      */ 
/*  561 */     JPanel wControls = new JPanel(new KJFlowLayout());
/*      */ 
/*  563 */     wControls.setOpaque(false);
/*      */ 
/*  565 */     wDisplay.add(wControls);
/*      */ 
/*  567 */     this.positionSlider = new JSlider(0, 0, 0);
/*  568 */     this.positionSlider.setOpaque(false);
/*      */ 
/*  570 */     wControls.add(this.positionSlider);
/*      */ 
/*  572 */     this.positionSlider.addChangeListener(new ChangeListener() {
/*      */       public void stateChanged(ChangeEvent pEvent) {
/*  574 */         if ((!KJJukeBox.this.positionIsAdjusting) && 
/*  575 */           (!KJJukeBox.this.positionSlider.getValueIsAdjusting()))
/*  576 */           KJJukeBox.player.seek(KJJukeBox.this.positionSlider.getValue());
/*      */       }
/*      */     });
/*  581 */     JPanel wPlaybackControls = new JPanel(
/*  582 */       new KJFlowLayout(KJFlowLayout.ORIENTATION_HORIZONTAL));
/*      */ 
/*  584 */     wPlaybackControls.setOpaque(false);
/*      */ 
/*  586 */     wControls.add(wPlaybackControls);
/*      */ 
/*  588 */     wPlaybackControls.add(KJUITools.createButton("<< Prev", this));
/*  589 */     wPlaybackControls.add(KJUITools.createButton("Play", this));
/*  590 */     wPlaybackControls.add(KJUITools.createButton("Pause", this));
/*  591 */     wPlaybackControls.add(KJUITools.createButton("Stop", this));
/*  592 */     wPlaybackControls.add(KJUITools.createButton("Next >>", this));
/*      */ 
/*  594 */     this.volumeSlider = new JSlider(1, 0, 100, 100);
/*      */ 
/*  596 */     this.volumeSlider.setOpaque(false);
/*      */ 
/*  598 */     this.volumeSlider.setPreferredSize(
/*  599 */       new Dimension(this.volumeSlider.getPreferredSize().width, 128));
/*      */ 
/*  601 */     getContentPane().add(this.volumeSlider);
/*      */ 
/*  603 */     this.volumeSlider.addChangeListener(new ChangeListener()
/*      */     {
/*      */       public void stateChanged(ChangeEvent pEvent) {
/*  606 */         KJJukeBox.player.setGain(KJJukeBox.this.volumeSlider.getValue());
/*      */ 
/*  608 */         if (!KJJukeBox.this.volumeIsAdjusting)
/*      */         {
/*  610 */           KJJukeBox.venue.setGain(KJJukeBox.this.volumeSlider.getValue());
/*      */ 
/*  612 */           KJJukeBox.this.volumeIsAdjusting = true;
/*      */ 
/*  614 */           KJJukeBox.this.microphonePriorityCheckMenuItem.setSelected(false);
/*  615 */           KJJukeBox.this.muteCheckMenuItem.setSelected(false);
/*      */ 
/*  617 */           KJJukeBox.this.volumeIsAdjusting = false;
/*      */         }
/*      */       }
/*      */     });
/*  624 */     trackListEditor = new KJEditTrackListsDialog(this);
/*      */ 
/*  627 */     this.storeClient_ = new KJStoreClientUI(this, trackListEditor, this.server_);
/*      */ 
/*  629 */     trackListEditor
/*  630 */       .setLocation(wSs.width - trackListEditor.getSize().width, 
/*  631 */       wSs.height - getSize().height - 
/*  632 */       trackListEditor.getSize().height - 32);
/*      */ 
/*  634 */     requestList = new KJRequestListDialog(this);
/*  635 */     dsp = new KJDSPDialog(this);
/*      */ 
/*  637 */     prepareMenus();
/*      */   }
/*      */ 
/*      */   private void prepareMenus()
/*      */   {
/*  643 */     JMenuBar wMenuBar = new JMenuBar();
/*  644 */     wMenuBar.setFont(wMenuBar.getFont().deriveFont(8.0F));
/*      */ 
/*  646 */     setJMenuBar(wMenuBar);
/*      */ 
/*  648 */     JMenu wFileMenu = KJUITools.addMenu(wMenuBar, "File", 'F');
/*      */ 
/*  650 */     KJUITools.addMenuItem(wFileMenu, KJUITools.createMenuItem(
/*  651 */       "New Venue...", 'N', this));
/*  652 */     KJUITools.addMenuItem(wFileMenu, KJUITools.createMenuItem(
/*  653 */       "Open Venue...", 'O', this));
/*  654 */     wFileMenu.addSeparator();
/*  655 */     KJUITools.addMenuItem(wFileMenu, 
/*  656 */       KJUITools.createMenuItem("Save Venue", 
/*  656 */       'S', this));
/*  657 */     KJUITools.addMenuItem(wFileMenu, KJUITools.createMenuItem(
/*  658 */       "Save Venue As...", 'A', this));
/*  659 */     wFileMenu.addSeparator();
/*  660 */     KJUITools.addMenuItem(wFileMenu, KJUITools.createMenuItem(
/*  661 */       "Preferrences...", 'P', this));
/*  662 */     wFileMenu.addSeparator();
/*  663 */     KJUITools.addMenuItem(wFileMenu, 
/*  664 */       KJUITools.createMenuItem("Exit", 'X', 
/*  664 */       this));
/*      */ 
/*  666 */     JMenu wViewMenu = KJUITools.addMenu(wMenuBar, "View", 'V');
/*      */ 
/*  668 */     JCheckBoxMenuItem wMi = (JCheckBoxMenuItem)KJUITools.addMenuItem(
/*  669 */       wViewMenu, 
/*  670 */       KJUITools.createCheckMenuItem("Track List Editor", 
/*  670 */       'T', this));
/*  671 */     trackListEditor.addComponentListener(new DialogVisibilityToggler(wMi));
/*      */ 
/*  673 */     wMi = (JCheckBoxMenuItem)KJUITools.addMenuItem(wViewMenu, 
/*  674 */       KJUITools.createCheckMenuItem("Request List", 'R', this));
/*  675 */     requestList.addComponentListener(new DialogVisibilityToggler(wMi));
/*      */ 
/*  677 */     wMi = (JCheckBoxMenuItem)KJUITools.addMenuItem(wViewMenu, 
/*  678 */       KJUITools.createCheckMenuItem("DSP", 'D', this));
/*  679 */     dsp.addComponentListener(new DialogVisibilityToggler(wMi));
/*      */ 
/*  681 */     wViewMenu.addSeparator();
/*  682 */     KJUITools.addMenuItem(wViewMenu, KJUITools.createMenuItem(
/*  683 */       "Track Properties...", 'O', this));
/*      */ 
/*  685 */     JMenu wPlaybackMenu = KJUITools.addMenu(wMenuBar, "Playback", 'P');
/*      */ 
/*  687 */     KJUITools.addMenuItem(wPlaybackMenu, 
/*  688 */       KJUITools.createMenuItem("Play", 
/*  688 */       'P', this));
/*  689 */     this.pauseCheckMenuItem = ((JCheckBoxMenuItem)KJUITools.addMenuItem(
/*  690 */       wPlaybackMenu, 
/*  691 */       KJUITools.createCheckMenuItem("Pause", 'U', this)));
/*  692 */     wPlaybackMenu.addSeparator();
/*  693 */     KJUITools.addMenuItem(wPlaybackMenu, KJUITools.createMenuItem(
/*  694 */       "Previous", 'V', this));
/*  695 */     KJUITools.addMenuItem(wPlaybackMenu, 
/*  696 */       KJUITools.createMenuItem("Next", 
/*  696 */       'N', this));
/*  697 */     KJUITools.addMenuItem(wPlaybackMenu, 
/*  698 */       KJUITools.createMenuItem("Repeat", 
/*  698 */       'R', this));
/*  699 */     wPlaybackMenu.addSeparator();
/*  700 */     KJUITools.addMenuItem(wPlaybackMenu, 
/*  701 */       KJUITools.createMenuItem("Stop", 
/*  701 */       'S', this));
/*  702 */     wPlaybackMenu.addSeparator();
/*      */ 
/*  704 */     this.microphonePriorityCheckMenuItem = ((JCheckBoxMenuItem)
/*  705 */       KJUITools.addMenuItem(wPlaybackMenu, KJUITools.createCheckMenuItem(
/*  706 */       "Microphone Priority", 'M', this)));
/*  707 */     this.muteCheckMenuItem = ((JCheckBoxMenuItem)
/*  708 */       KJUITools.addMenuItem(wPlaybackMenu, KJUITools.createCheckMenuItem(
/*  709 */       "Mute", 'U', this)));
/*      */ 
/*  711 */     JMenu wOptionsMenu = KJUITools.addMenu(wMenuBar, "Options", 'O');
/*      */ 
/*  713 */     this.fadeWhenNavigatingCheckMenuItem = ((JCheckBoxMenuItem)
/*  714 */       KJUITools.addMenuItem(wOptionsMenu, KJUITools.createCheckMenuItem(
/*  715 */       "Fade When Navigating", 'F', this)));
/*      */ 
/*  717 */     this.markPlayedTracks = ((JCheckBoxMenuItem)KJUITools.addMenuItem(
/*  718 */       wOptionsMenu, KJUITools.createCheckMenuItem(
/*  719 */       "Mark Played Tracks", 'M', this)));
/*  720 */     this.markPlayedTracks.setSelected(player.isMarkingPlayedTracks());
/*      */ 
/*  722 */     JMenu wHelpMenu = KJUITools.addMenu(wMenuBar, "Help", 'H');
/*      */ 
/*  724 */     wMi = (JCheckBoxMenuItem)KJUITools.addMenuItem(wHelpMenu, 
/*  725 */       KJUITools.createCheckMenuItem("Documentation...", 'D', this));
/*      */ 
/*  727 */     wHelpMenu.addSeparator();
/*  728 */     KJUITools.addMenuItem(wHelpMenu, 
/*  729 */       KJUITools.createMenuItem("About...", 
/*  729 */       'A', this));
/*      */   }
/*      */ 
/*      */   public static void removeVenueChangeListener(KJVenueChangeListener pListener)
/*      */   {
/*  734 */     venueChangeListeners.remove(pListener);
/*      */   }
/*      */ 
/*      */   private void openVenue(File pLocation)
/*      */   {
/*      */     try
/*      */     {
/*  741 */       Document wVenueDocument = 
/*  742 */         KJXmlToolkit.loadDocument(new FileInputStream(pLocation));
/*      */ 
/*  744 */       KJVenue wVenue = new KJVenue();
/*      */ 
/*  746 */       wVenue.readXml(wVenueDocument.getDocumentElement());
/*      */ 
/*  748 */       updateVenue(wVenue);
/*      */     }
/*      */     catch (Exception pEx)
/*      */     {
/*  752 */       pEx.printStackTrace();
/*      */ 
/*  754 */       JOptionPane.showMessageDialog(this, pEx.toString(), 
/*  755 */         "Error Opening Venue", 0);
/*      */     }
/*      */   }
/*      */ 
/*      */   private File saveVenue(File pLocation)
/*      */   {
/*  763 */     if (pLocation == null)
/*      */     {
/*  765 */       File wFile = KJUITools.createSingleFileChooser(this, "Save Venue", 
/*  766 */         new VenueFileFilter(null));
/*      */ 
/*  768 */       if (wFile != null)
/*  769 */         pLocation = wFile;
/*      */       else {
/*  771 */         return null;
/*      */       }
/*      */ 
/*      */     }
/*      */ 
/*  776 */     pLocation = validateExtension(pLocation, ".kjv");
/*      */     try
/*      */     {
/*  780 */       Document wVenue = KJXmlToolkit.newDocument("Venue");
/*      */ 
/*  782 */       venue.writeXml(wVenue.getDocumentElement());
/*      */ 
/*  784 */       KJXmlToolkit.saveDocument(wVenue, new FileOutputStream(pLocation));
/*      */     }
/*      */     catch (Exception pEx)
/*      */     {
/*  788 */       JOptionPane.showMessageDialog(this, pEx.toString(), 
/*  789 */         "Error Saving Venue", 0);
/*      */ 
/*  791 */       return null;
/*      */     }
/*      */ 
/*  795 */     return pLocation;
/*      */   }
/*      */ 
/*      */   private void loadCurrentState()
/*      */   {
/*  801 */     String wHomeDir = System.getProperty("user.home");
/*      */ 
/*  803 */     File wKjdf = new File(wHomeDir, ".kj/KJJukeBox.dat");
/*      */     try
/*      */     {
/*  807 */       Element wKJSettings = KJXmlToolkit.loadDocument(
/*  808 */         new FileInputStream(wKjdf)).getDocumentElement();
/*      */ 
/*  810 */       wKJSettings.setAttribute("version", "v0.45");
/*      */ 
/*  812 */       String wVfn = KJXmlToolkit.get(wKJSettings, "VenueLocation");
/*      */ 
/*  814 */       if (wVfn != null) {
/*  815 */         venueLocation = new File(wVfn);
/*      */       }
/*      */ 
/*  818 */       KJXmlToolkit.getObject(wKJSettings, "JukeBox", this);
/*  819 */       setVisible(true);
/*      */ 
/*  821 */       KJXmlToolkit.getObject(wKJSettings, "TrackListEditor", 
/*  822 */         trackListEditor);
/*  823 */       KJXmlToolkit.getObject(wKJSettings, "RequestList", requestList);
/*  824 */       KJXmlToolkit.getObject(wKJSettings, "DSP", dsp);
/*      */ 
/*  828 */       this.storeClient_.setVisible(true);
/*      */     }
/*      */     catch (Exception pEx)
/*      */     {
/*  832 */       setVisible(true);
/*      */ 
/*  834 */       trackListEditor.setVisible(true);
/*      */ 
/*  836 */       this.storeClient_.setVisible(true);
/*      */     }
/*      */     finally
/*      */     {
/*  841 */       if (venueLocation != null)
/*  842 */         openVenue(venueLocation);
/*      */       else
/*  844 */         updateVenue(new KJVenue(true));
/*      */     }
/*      */   }
/*      */ 
/*      */   private void saveCurrentState()
/*      */   {
/*  853 */     String wHomeDir = System.getProperty("user.home");
/*      */     try
/*      */     {
/*  857 */       venueLocation = saveVenue(venueLocation);
/*      */ 
/*  859 */       Element wKJSettings = KJXmlToolkit.newDocument("KJ")
/*  860 */         .getDocumentElement();
/*      */ 
/*  862 */       wKJSettings.setAttribute("version", "v0.45");
/*      */ 
/*  864 */       if (venueLocation != null) {
/*  865 */         KJXmlToolkit.set(wKJSettings, "VenueLocation", venueLocation);
/*      */       }
/*      */ 
/*  868 */       KJXmlToolkit.set(wKJSettings, "JukeBox", this);
/*  869 */       KJXmlToolkit.set(wKJSettings, "TrackListEditor", trackListEditor);
/*  870 */       KJXmlToolkit.set(wKJSettings, "RequestList", requestList);
/*  871 */       KJXmlToolkit.set(wKJSettings, "DSP", dsp);
/*      */ 
/*  873 */       File wKjdf = new File(wHomeDir, ".kj/KJJukeBox.dat");
/*      */ 
/*  875 */       wKjdf.getParentFile().mkdirs();
/*      */ 
/*  877 */       KJXmlToolkit.saveDocument(wKJSettings.getOwnerDocument(), 
/*  878 */         new FileOutputStream(wKjdf));
/*      */     }
/*      */     catch (Exception pEx) {
/*  881 */       pEx.printStackTrace();
/*      */     }
/*      */   }
/*      */ 
/*      */   private void showNextTrack()
/*      */   {
/*  888 */     KJTrack wNextTrack = getNextTrack();
/*      */ 
/*  890 */     if (wNextTrack != null)
/*  891 */       this.labelNextTrack.setText("Next: " + wNextTrack.toString());
/*      */     else
/*  893 */       this.labelNextTrack.setText("Next: N/A");
/*      */   }
/*      */ 
/*      */   private File validateExtension(File pFile, String pExtension)
/*      */   {
/*  939 */     String wVn = pFile.toString();
/*      */ 
/*  941 */     if (wVn.length() > pExtension.length())
/*      */     {
/*  943 */       if (wVn.substring(wVn.length() - pExtension.length())
/*  943 */         .equalsIgnoreCase(".kjv")) {
/*  944 */         return pFile;
/*      */       }
/*      */     }
/*  947 */     return new File(wVn + pExtension);
/*      */   }
/*      */ 
/*      */   private void btnPrev(JButton pButton)
/*      */   {
/*  952 */     player.previous();
/*      */   }
/*      */ 
/*      */   private void btnPlay(JButton pButton) {
/*  956 */     player.play();
/*      */   }
/*      */ 
/*      */   private void btnPause(JButton pButton) {
/*  960 */     player.pause();
/*      */   }
/*      */ 
/*      */   private void btnStop(JButton pButton) {
/*  964 */     player.stop();
/*      */   }
/*      */ 
/*      */   private void btnNext(JButton pButton) {
/*  968 */     player.next();
/*      */   }
/*      */ 
/*      */   private void miFileNewVenue(JMenuItem pItem)
/*      */   {
/*  973 */     venueLocation = null;
/*      */ 
/*  975 */     updateVenue(new KJVenue(true));
/*      */   }
/*      */ 
/*      */   private void miFileOpenVenue(JMenuItem pItem)
/*      */   {
/*  981 */     if (venueLocation != null) {
/*  982 */       KJUITools.setFileChooserBrowseLocation(venueLocation);
/*      */     }
/*      */ 
/*  985 */     File wFile = KJUITools.createSingleFileChooser(this, "Open", 
/*  986 */       new VenueFileFilter(null));
/*      */ 
/*  988 */     if (wFile != null)
/*      */     {
/*  990 */       venueLocation = wFile;
/*      */ 
/*  992 */       openVenue(wFile);
/*      */     }
/*      */   }
/*      */ 
/*      */   private void miFileSaveVenue(JMenuItem pItem)
/*      */   {
/*  999 */     venueLocation = saveVenue(venueLocation);
/*      */   }
/*      */ 
/*      */   private void miFileSaveVenueAs(JMenuItem pItem) {
/* 1003 */     venueLocation = saveVenue(null);
/*      */   }
/*      */ 
/*      */   private void miFilePreferrences(JMenuItem pItem)
/*      */   {
/* 1008 */     new KJVenuePreferrences(this, venue);
/*      */ 
/* 1010 */     updateJukeBox();
/* 1011 */     updatePlayerSettings();
/*      */   }
/*      */ 
/*      */   private void miFileExit(JMenuItem pItem)
/*      */   {
/* 1016 */     exitApplication();
/*      */   }
/*      */ 
/*      */   private void miHelpAbout(JMenuItem pItem)
/*      */   {
/* 1021 */     JOptionPane.showMessageDialog(this, "KJ v0.45 (August 2005)\r\n\r\nDeveloped by: K. Fudalewski and N. Mass?\r\nE-Mail: sirk_sytes@hotmail.com\r\nWebsite: http://sirk.sytes.net", 
/* 1025 */       "About...", 
/* 1026 */       -1);
/*      */   }
/*      */ 
/*      */   private void mcHelpDocumentation(JCheckBoxMenuItem pItem)
/*      */   {
/* 1032 */     if (documentation == null)
/*      */     {
/* 1034 */       documentation = new KJHelpDocumentation(this);
/* 1035 */       documentation.addComponentListener(
/* 1036 */         new DialogVisibilityToggler(pItem));
/*      */     }
/*      */     else {
/* 1039 */       documentation.setVisible(pItem.isSelected());
/*      */     }
/*      */   }
/*      */ 
/*      */   private void mcOptionsFadeWhenNavigating(JCheckBoxMenuItem pItem)
/*      */   {
/* 1046 */     venue.setNavigationCrossFadingEnabled(pItem.isSelected());
/* 1047 */     updatePlayerSettings();
/*      */   }
/*      */ 
/*      */   private void mcOptionsMarkPlayedTracks(JCheckBoxMenuItem pItem)
/*      */   {
/* 1052 */     player.setMarkPlayedTracks(pItem.isSelected());
/*      */   }
/*      */ 
/*      */   private void mcPlaybackMicrophonePriority(JCheckBoxMenuItem pItem)
/*      */   {
/* 1057 */     if (pItem.isSelected())
/*      */     {
/* 1059 */       this.volumeIsAdjusting = true;
/*      */ 
/* 1061 */       player.fadeGain(venue.getMicrophonePriorityGain(), 1000L);
/* 1062 */       this.volumeSlider.setValue((int)venue.getMicrophonePriorityGain());
/*      */ 
/* 1064 */       this.muteCheckMenuItem.setSelected(false);
/*      */ 
/* 1066 */       this.volumeIsAdjusting = false;
/*      */     }
/* 1068 */     else if (!this.volumeIsAdjusting)
/*      */     {
/* 1070 */       this.volumeIsAdjusting = true;
/*      */ 
/* 1072 */       player.fadeGain(venue.getGain(), 1000L);
/* 1073 */       this.volumeSlider.setValue((int)venue.getGain());
/*      */ 
/* 1075 */       this.volumeIsAdjusting = false;
/*      */     }
/*      */   }
/*      */ 
/*      */   private void mcPlaybackMute(JCheckBoxMenuItem pItem)
/*      */   {
/* 1083 */     if (pItem.isSelected())
/*      */     {
/* 1085 */       this.volumeIsAdjusting = true;
/*      */ 
/* 1087 */       player.fadeGain(0.0F, 1000L);
/* 1088 */       this.volumeSlider.setValue(0);
/*      */ 
/* 1090 */       this.microphonePriorityCheckMenuItem.setSelected(false);
/*      */ 
/* 1092 */       this.volumeIsAdjusting = false;
/*      */     }
/* 1094 */     else if (!this.volumeIsAdjusting)
/*      */     {
/* 1096 */       this.volumeIsAdjusting = true;
/*      */ 
/* 1098 */       player.fadeGain(venue.getGain(), 1000L);
/* 1099 */       this.volumeSlider.setValue((int)venue.getGain());
/*      */ 
/* 1101 */       this.volumeIsAdjusting = false;
/*      */     }
/*      */   }
/*      */ 
/*      */   private void mcPlaybackPause(JCheckBoxMenuItem pItem)
/*      */   {
/* 1107 */     if (!this.pauseIsAdjusting)
/* 1108 */       player.pause();
/*      */   }
/*      */ 
/*      */   private void miPlaybackRepeat(JMenuItem pItem)
/*      */   {
/* 1113 */     player.repeat();
/*      */   }
/*      */ 
/*      */   private void miPlaybackPlay(JMenuItem pItem) {
/* 1117 */     player.play();
/*      */   }
/*      */ 
/*      */   private void miPlaybackPrevious(JMenuItem pItem) {
/* 1121 */     player.previous();
/*      */   }
/*      */ 
/*      */   private void miPlaybackNext(JMenuItem pItem) {
/* 1125 */     player.next();
/*      */   }
/*      */ 
/*      */   private void miPlaybackStop(JMenuItem pItem) {
/* 1129 */     player.stop();
/*      */   }
/*      */ 
/*      */   private void mcViewTrackListEditor(JCheckBoxMenuItem pItem) {
/* 1133 */     trackListEditor.setVisible(pItem.isSelected());
/*      */   }
/*      */ 
/*      */   private void mcViewRequestList(JCheckBoxMenuItem pItem) {
/* 1137 */     requestList.setVisible(pItem.isSelected());
/*      */   }
/*      */ 
/*      */   private void mcViewDSP(JCheckBoxMenuItem pItem) {
/* 1141 */     dsp.setVisible(pItem.isSelected());
/*      */   }
/*      */ 
/*      */   private void miViewTrackProperties(JMenuItem pItem)
/*      */   {
/* 1146 */     KJTrack wTrack = player.getCurrentTrack();
/*      */ 
/* 1148 */     if (wTrack != null)
/* 1149 */       new KJTrackProperties(this, new KJTrack[] { wTrack });
/*      */   }
/*      */ 
/*      */   public static void main(String[] args)
/*      */     throws Exception
/*      */   {
/* 1155 */     new KJJukeBox();
/*      */   }
/*      */ 
/*      */   public void readXml(Element pSource)
/*      */   {
/* 1273 */     Element wBounds = KJXmlToolkit.find(pSource, "Bounds");
/*      */ 
/* 1275 */     setBounds(Integer.parseInt(wBounds.getAttribute("x")), 
/* 1276 */       Integer.parseInt(wBounds.getAttribute("y")), Integer.parseInt(
/* 1277 */       wBounds.getAttribute("width")), Integer.parseInt(wBounds
/* 1278 */       .getAttribute("height")));
/*      */   }
/*      */ 
/*      */   public void writeXml(Element pTarget)
/*      */   {
/* 1289 */     Element wBounds = KJXmlToolkit.add(pTarget, "Bounds");
/*      */ 
/* 1291 */     wBounds.setAttribute("x", String.valueOf(getBounds().x));
/* 1292 */     wBounds.setAttribute("y", String.valueOf(getBounds().y));
/* 1293 */     wBounds.setAttribute("width", String.valueOf(getBounds().width));
/* 1294 */     wBounds.setAttribute("height", String.valueOf(getBounds().height));
/*      */   }
/*      */ 
/*      */   private class DialogVisibilityToggler extends ComponentAdapter
/*      */   {
/*      */     private JCheckBoxMenuItem cbmi;
/*      */ 
/*      */     public DialogVisibilityToggler(JCheckBoxMenuItem pCbmi)
/*      */     {
/* 1163 */       this.cbmi = pCbmi;
/*      */     }
/*      */ 
/*      */     public void componentHidden(ComponentEvent pEvent) {
/* 1167 */       this.cbmi.setSelected(false);
/*      */     }
/*      */ 
/*      */     public void componentShown(ComponentEvent pEvent) {
/* 1171 */       this.cbmi.setSelected(true);
/*      */     }
/*      */   }
/*      */ 
/*      */   private class PlayerListener implements KJPlayerListener {
/*      */     private PlayerListener() {
/*      */     }
/*      */ 
/*      */     public void playerEvent(KJPlayerEvent pEvent) {
/* 1180 */       if (pEvent.getType() == 1)
/*      */       {
/* 1182 */         KJJukeBox.this.labelSongTitle.setForeground(Color.green);
/*      */ 
/* 1184 */         KJJukeBox.this.labelSongTitle.setText(pEvent.getTrack().toString());
/*      */ 
/* 1186 */         KJJukeBox.this.setTitle("KJ - [ " + pEvent.getTrack().toString() + " ]");
/*      */ 
/* 1188 */         KJJukeBox.this.positionIsAdjusting = true;
/*      */ 
/* 1190 */         KJJukeBox.this.positionSlider.setMinimum(
/* 1191 */           (int)pEvent.getTrack().getAudioDataProvider().startPosition());
/* 1192 */         KJJukeBox.this.positionSlider.setMaximum(
/* 1193 */           (int)pEvent.getTrack().getAudioDataProvider().endPosition());
/*      */ 
/* 1195 */         KJJukeBox.this.positionIsAdjusting = false;
/*      */ 
/* 1197 */         KJJukeBox.this.pauseIsAdjusting = true;
/* 1198 */         KJJukeBox.this.pauseCheckMenuItem.setSelected(false);
/* 1199 */         KJJukeBox.this.pauseIsAdjusting = false;
/*      */ 
/* 1202 */         KJJukeBox.venue.prepareNextTrack();
/*      */ 
/* 1204 */         KJJukeBox.this.showNextTrack();
/*      */       }
/* 1206 */       else if (pEvent.getType() == 2)
/*      */       {
/* 1208 */         KJJukeBox.this.labelSongTitle.setForeground(Color.green);
/*      */ 
/* 1210 */         KJJukeBox.this.labelSongTitle.setText(pEvent.getTrack().toString() + 
/* 1211 */           " <Paused>");
/*      */ 
/* 1213 */         KJJukeBox.this
/* 1214 */           .setTitle("KJ - [ " + pEvent.getTrack().toString() + 
/* 1214 */           " ] <Paused>");
/*      */ 
/* 1216 */         KJJukeBox.this.pauseIsAdjusting = true;
/* 1217 */         KJJukeBox.this.pauseCheckMenuItem.setSelected(true);
/* 1218 */         KJJukeBox.this.pauseIsAdjusting = false;
/*      */       }
/* 1220 */       else if (pEvent.getType() == 3)
/*      */       {
/* 1222 */         KJJukeBox.this.labelSongTitle.setForeground(Color.red);
/*      */ 
/* 1224 */         KJJukeBox.this.labelSongTitle.setText("[No Track] <Stopped>");
/*      */ 
/* 1226 */         KJJukeBox.this.setTitle("KJ v0.45");
/*      */ 
/* 1228 */         KJJukeBox.this.positionIsAdjusting = true;
/*      */ 
/* 1230 */         KJJukeBox.this.positionSlider.setMinimum(0);
/* 1231 */         KJJukeBox.this.positionSlider.setMaximum(0);
/* 1232 */         KJJukeBox.this.positionSlider.setValue(0);
/*      */ 
/* 1234 */         KJJukeBox.this.positionIsAdjusting = false;
/*      */       }
/* 1236 */       else if (pEvent.getType() == 5)
/*      */       {
/* 1238 */         KJTrack wTrack = pEvent.getTrack();
/*      */ 
/* 1240 */         wTrack.setRequested(false);
/*      */ 
/* 1242 */         KJJukeBox.venue.getRequestList().removeTrack(pEvent.getTrack());
/*      */       }
/* 1244 */       else if (pEvent.getType() == 6) {
/* 1245 */         KJJukeBox.this.showNextTrack();
/* 1246 */       } else if (pEvent.getType() == 4) {
/* 1247 */         KJJukeBox.this.labelSongTitle.setForeground(Color.yellow);
/*      */       }
/*      */     }
/*      */   }
/*      */ 
/*      */   private class VenueFileFilter extends FileFilter {
/*      */     private VenueFileFilter() {
/*      */     }
/*      */ 
/*      */     public boolean accept(File pFile) {
/* 1257 */       return (pFile.isDirectory()) || (pFile.getName().endsWith(".kjv"));
/*      */     }
/*      */ 
/*      */     public String getDescription() {
/* 1261 */       return "KJ Venue File (*.kjv)";
/*      */     }
/*      */   }
/*      */ }

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.ui.KJJukeBox
 * JD-Core Version:    0.6.0
 */