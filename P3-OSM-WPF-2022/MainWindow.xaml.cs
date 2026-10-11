using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using MapControl;
using LP_OSM_2223_WPF;

namespace LS_OSM_2223_WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct PoiBridge
        {
            public int i;
            public double lat;
            public double lon;
            public int color;
            public IntPtr title;
        }

        public struct Way
        {
            public double[] lat;
            public double[] lon;
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr AllocateResultOfStrategyArray(IntPtr length);

        [DllImport("P3-OSM-DLL-2022.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void loadOSMData(char[] map);

        [DllImport("P3-OSM-DLL-2022.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void renderPois(AllocateResultOfStrategyArray allocator);

        [DllImport("P3-OSM-DLL-2022.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool renderWays(int offset, AllocateResultOfStrategyArray allocatorLats, AllocateResultOfStrategyArray allocatorLons);

        [DllImport("P3-OSM-DLL-2022.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void shortestPath(int idxFrom, int idxTo, AllocateResultOfStrategyArray allocatorLats, AllocateResultOfStrategyArray allocatorLons);

        private MapItemsControl elementsLayer;
        private static BackgroundWorker backgroundWorker;
        private static BackgroundWorker backgroundWorker4CamiMesCurt;
        private FileSystemWatcher watcher;

        private PoiBridge[] poiBridges = new List<PoiBridge>().ToArray();
        private List<Way> ways = new List<Way>();


        private int idxFrom;
        private int idxTo;            

        private static char[] loadedMap;

        public MainWindow()
        {
            InitializeComponent();

            InitializeMap();
            InitializeLog();
            InitializeWorker();

            // 2nd part
            InitializeWorkerForCamiMesCurt();
        }

        // Prepara mapa amb el Tile de OSM
        private void InitializeMap()
        {
            elementsLayer = new MapItemsControl();
            mapControl.Children.Add(elementsLayer);
            mapControl.Center = new Location(41.4996, 2.1105);
            mapControl.ZoomLevel = 13;
        }

        // Prepara Log, on permetria llegir un fitxer que s'escriu a dins de la DLL
        private void InitializeLog()
        {
            watcher = new FileSystemWatcher();
            watcher.Path = ".";
            watcher.Filter = "Log.txt";
            watcher.Changed += Watcher_Changed;
            watcher.EnableRaisingEvents = true;
            watcher.NotifyFilter = NotifyFilters.LastWrite;
        }

        // Prepara Thread per carrega de OSM
        private void InitializeWorker()
        {
            backgroundWorker = new BackgroundWorker();
            backgroundWorker.DoWork += BackgroundWorker_DoWork;
            backgroundWorker.RunWorkerCompleted += BackgroundWorker_RunWorkerCompleted;
        }

        // Prepara Thread per calcul de cami mes curt
        private void InitializeWorkerForCamiMesCurt()
        {
            backgroundWorker4CamiMesCurt = new BackgroundWorker();
            backgroundWorker4CamiMesCurt.DoWork += BackgroundWorker4CamiMesCurt_DoWork;
            backgroundWorker4CamiMesCurt.RunWorkerCompleted += BackgroundWorker4CamiMesCurt_RunWorkerCompleted;
        }

        // Listener que el Thread del Cami Mes Curt ja ha finalitzat
        private void BackgroundWorker4CamiMesCurt_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            logText.Text += "Acabat el càlcul del camí més curt." + Environment.NewLine;
        }

        // Carrega del treball del Thread Calcul del Cami Mes Curt
        private void BackgroundWorker4CamiMesCurt_DoWork(object sender, DoWorkEventArgs e)
        {
            List<Way> camiMesCurt = new List<Way>();

            if (idxFrom == idxTo) {
                return;
            }

            using (var pLat = new PinnedArray<double>())
            {
                using (var pLon = new PinnedArray<double>())
                {
                    shortestPath(idxFrom, idxTo, pLat.CreateArray, pLon.CreateArray);
                    Way shortestWay = new Way { lat = pLat.Array, lon = pLon.Array };
                    camiMesCurt.Add(shortestWay);
                }
            }

            if (camiMesCurt.Count > 0) { 
                ways = camiMesCurt;
            }

        }

        // Listener que el Thread de Carrega OSM ja ha acabat
        private void BackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            logText.Text += "Acabada la càrrega, pots visualitzar." + Environment.NewLine;
            this.Cursor = Cursors.Arrow;
            mapControl.Cursor = Cursors.Arrow;

            foreach ( PoiBridge poiBridge in poiBridges){
                cmbFromPois.Items.Add(Marshal.PtrToStringAnsi(poiBridge.title));
                cmbToPois.Items.Add(Marshal.PtrToStringAnsi(poiBridge.title));
            }

            if (cmbFromPois.Items.Count > 0 && cmbToPois.Items.Count > 0) {
                cmbFromPois.SelectedIndex = 0;
                cmbToPois.SelectedIndex = 0;
            }
        }

        // Carrega de treball de carrega del OSM  
        private void BackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            loadOSMData(loadedMap);

            using (var pa = new PinnedArray<PoiBridge>())
            {
                renderPois(pa.CreateArray);
                poiBridges = pa.Array;
            }

            bool nextWay = false;
            int i = 0;

            do
            {
                using (var pLat = new PinnedArray<double>())
                {
                    using (var pLon = new PinnedArray<double>())
                    {
                        nextWay = renderWays(i, pLat.CreateArray, pLon.CreateArray);
                        Way way = new Way { lat = pLat.Array, lon = pLon.Array };
                        ways.Add(way);
                        i++;
                    }
                }
            } while (nextWay);
        }

        // Representa punt de interes amb un marcador MapItem natif de WPF
        private MapItem Representa_MapItem(double latitude, double longitude, string title, System.Windows.Media.Color color)
        {
            var ellipse = new Ellipse
            {
                Width = 14,
                Height = 14,
                Fill = new SolidColorBrush(color),
                Stroke = System.Windows.Media.Brushes.Black,
                StrokeThickness = 1.5,
                ToolTip = title
            };

            var mapItem = new MapItem
            {
                Location = new Location(latitude, longitude),
                Content = ellipse
            };

            return mapItem;
        }

        private void Watcher_Changed(object sender, FileSystemEventArgs e)
        {
            watcher.EnableRaisingEvents = false;

            this.Dispatcher.BeginInvoke(new Action(() => {
                FileStream fileStream = new FileStream("Log.txt", FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                StreamReader reader = new StreamReader(fileStream);

                logText.Text += Environment.NewLine + reader.ReadToEnd();

                reader.Close();

                watcher.EnableRaisingEvents = true;
            }));
        }

        // Inici del Thread calcul del Cami Mes Curt
        private void ShortestPath_Click(object sender, RoutedEventArgs e)
        {
            idxFrom = cmbFromPois.SelectedIndex;
            idxTo = cmbToPois.SelectedIndex;
            if (idxFrom == idxTo) {
                return;
            }
            backgroundWorker4CamiMesCurt.RunWorkerAsync();
        }

        // Visualitza Punts De Interes que s'han carregat anteriorment
        private void VisualitzaPdis_Click(object sender, RoutedEventArgs e)
        {
            var items = new List<MapItem>();

            foreach (var item in poiBridges)
            {
                int color = item.color;
                var mediaColor = System.Windows.Media.Color.FromRgb(
                    (byte)((color >> 16) & 0xff),
                    (byte)((color >> 8) & 0xff),
                    (byte)(color & 0xff)
                );
                string title = Marshal.PtrToStringAnsi(item.title);
                items.Add(Representa_MapItem(item.lat, item.lon, title, mediaColor));
            }

            elementsLayer.ItemsSource = items;

            if (items.Count > 0)
            {
                mapControl.Center = items[0].Location;
            }
        }

        // Visualitza Camins que s'han carregat anteriorment
        private void VisualitzaCamins_Click(object sender, RoutedEventArgs e)
        {
            // Eliminar polilineas existentes dejando solo MapItemsControl
            var polylinesToRemove = new List<UIElement>();
            foreach (UIElement child in mapControl.Children)
            {
                if (child is MapPolyline)
                {
                    polylinesToRemove.Add(child);
                }
            }
            foreach (var p in polylinesToRemove)
            {
                mapControl.Children.Remove(p);
            }

            foreach (var way in ways)
            {
                var locations = new LocationCollection();
                for (int i = 0; i < way.lat.Length; i++)
                {
                    locations.Add(new Location(way.lat[i], way.lon[i]));
                }

                var mapPolyline = new MapPolyline
                {
                    Stroke = System.Windows.Media.Brushes.Green,
                    StrokeThickness = 3,
                    Locations = locations
                };

                mapControl.Children.Add(mapPolyline);
            }
        }

        // Neteja dels elements que formen en el mapa
        private void Clear_Click(object sender, RoutedEventArgs e)
        {
            elementsLayer.ItemsSource = null;

            var toRemove = new List<UIElement>();
            foreach (UIElement child in mapControl.Children)
            {
                if (child is MapPolyline)
                {
                    toRemove.Add(child);
                }
            }
            foreach (var item in toRemove)
            {
                mapControl.Children.Remove(item);
            }
        }

        // Punt d'inici de carrega del fitxer OSM. Neteja tots els elements i fa un clear de les dades emmagatzemades. 
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            bool showDialog = (bool)openFileDialog.ShowDialog();

            if (showDialog)
            {
                poiBridges = new List<PoiBridge>().ToArray();
                ways.Clear();
                cmbFromPois.Items.Clear();
                cmbToPois.Items.Clear();

                Clear_Click(sender, e);

                string fileName = openFileDialog.FileName + "\0";
                loadedMap = fileName.ToCharArray();

                logText.Text = "Carregant..." + Environment.NewLine;
                this.Cursor = Cursors.Wait;
                mapControl.Cursor = Cursors.Wait;
                backgroundWorker.RunWorkerAsync();
            }
            else
            {
                logText.Text = "ERROR: en càrrega del fitxer!" + Environment.NewLine;
            }
        }

    }
}
