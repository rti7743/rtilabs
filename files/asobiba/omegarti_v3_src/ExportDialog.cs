using System;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using Travis.Util;
using Zanetti.Data;
using Zanetti.Config;
using Zanetti.SystemTrading;

namespace Zanetti.Forms {
    internal class ExportDialog : Form {
        public ExportDialog() {
            InitializeComponent();
            InitUI();
        }
        private BookmarkFolder _bookmarkFolder;

        private Label _lFormat;
        private ComboBox _formatBox;
        private Label _lTarget;
        private ComboBox _targetBox;
        private Button _okButton;
        private Button _cancelButton;

        private void InitializeComponent()
        {
            this._lFormat = new System.Windows.Forms.Label();
            this._formatBox = new System.Windows.Forms.ComboBox();
            this._lTarget = new System.Windows.Forms.Label();
            this._targetBox = new System.Windows.Forms.ComboBox();
            this._okButton = new System.Windows.Forms.Button();
            this._cancelButton = new System.Windows.Forms.Button();
            this._lBookmark = new System.Windows.Forms.Label();
            this._lDirectory = new System.Windows.Forms.Label();
            this._directoryBox = new System.Windows.Forms.TextBox();
            this._selectDirectory = new System.Windows.Forms.Button();
            this._selectBookmarkButton = new System.Windows.Forms.Button();
            this._bookmarkBox = new System.Windows.Forms.TextBox();
            this.status1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _lFormat
            // 
            this._lFormat.AutoSize = true;
            this._lFormat.Location = new System.Drawing.Point(13, 13);
            this._lFormat.Name = "_lFormat";
            this._lFormat.Size = new System.Drawing.Size(70, 12);
            this._lFormat.TabIndex = 0;
            this._lFormat.Text = "フォーマット(&F)";
            this._lFormat.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _formatBox
            // 
            this._formatBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._formatBox.Items.AddRange(new object[] {
            "CSV",
            "XML"});
            this._formatBox.Location = new System.Drawing.Point(121, 13);
            this._formatBox.Name = "_formatBox";
            this._formatBox.Size = new System.Drawing.Size(241, 20);
            this._formatBox.TabIndex = 1;
            // 
            // _lTarget
            // 
            this._lTarget.AutoSize = true;
            this._lTarget.Location = new System.Drawing.Point(13, 34);
            this._lTarget.Name = "_lTarget";
            this._lTarget.Size = new System.Drawing.Size(44, 12);
            this._lTarget.TabIndex = 2;
            this._lTarget.Text = "対象(&T)";
            this._lTarget.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _targetBox
            // 
            this._targetBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._targetBox.Location = new System.Drawing.Point(121, 34);
            this._targetBox.Name = "_targetBox";
            this._targetBox.Size = new System.Drawing.Size(241, 20);
            this._targetBox.TabIndex = 3;
            this._targetBox.SelectedIndexChanged += new System.EventHandler(this.OnTargetChanged);
            // 
            // _okButton
            // 
            this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._okButton.Location = new System.Drawing.Point(205, 111);
            this._okButton.Name = "_okButton";
            this._okButton.Size = new System.Drawing.Size(75, 23);
            this._okButton.TabIndex = 4;
            this._okButton.Text = "OK";
            this._okButton.Click += new System.EventHandler(this.OnOK);
            // 
            // _cancelButton
            // 
            this._cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._cancelButton.Location = new System.Drawing.Point(287, 113);
            this._cancelButton.Name = "_cancelButton";
            this._cancelButton.Size = new System.Drawing.Size(75, 23);
            this._cancelButton.TabIndex = 5;
            this._cancelButton.Text = "キャンセル";
            // 
            // _lBookmark
            // 
            this._lBookmark.AutoSize = true;
            this._lBookmark.Location = new System.Drawing.Point(13, 55);
            this._lBookmark.Name = "_lBookmark";
            this._lBookmark.Size = new System.Drawing.Size(108, 12);
            this._lBookmark.TabIndex = 6;
            this._lBookmark.Text = "出力元ブックマーク(&B)";
            this._lBookmark.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _lDirectory
            // 
            this._lDirectory.AutoSize = true;
            this._lDirectory.Location = new System.Drawing.Point(13, 85);
            this._lDirectory.Name = "_lDirectory";
            this._lDirectory.Size = new System.Drawing.Size(106, 12);
            this._lDirectory.TabIndex = 7;
            this._lDirectory.Text = "出力先ディレクトリ(&D)";
            this._lDirectory.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _directoryBox
            // 
            this._directoryBox.Location = new System.Drawing.Point(121, 85);
            this._directoryBox.Name = "_directoryBox";
            this._directoryBox.Size = new System.Drawing.Size(220, 19);
            this._directoryBox.TabIndex = 8;
            // 
            // _selectDirectory
            // 
            this._selectDirectory.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._selectDirectory.Location = new System.Drawing.Point(343, 85);
            this._selectDirectory.Name = "_selectDirectory";
            this._selectDirectory.Size = new System.Drawing.Size(19, 19);
            this._selectDirectory.TabIndex = 9;
            this._selectDirectory.Text = "...";
            this._selectDirectory.Click += new System.EventHandler(this.OnSelectDirectory);
            // 
            // _selectBookmarkButton
            // 
            this._selectBookmarkButton.Enabled = false;
            this._selectBookmarkButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._selectBookmarkButton.Location = new System.Drawing.Point(343, 55);
            this._selectBookmarkButton.Name = "_selectBookmarkButton";
            this._selectBookmarkButton.Size = new System.Drawing.Size(19, 19);
            this._selectBookmarkButton.TabIndex = 11;
            this._selectBookmarkButton.Text = "...";
            this._selectBookmarkButton.Click += new System.EventHandler(this.OnSelectBookmarkFolder);
            // 
            // _bookmarkBox
            // 
            this._bookmarkBox.Enabled = false;
            this._bookmarkBox.Location = new System.Drawing.Point(121, 55);
            this._bookmarkBox.Name = "_bookmarkBox";
            this._bookmarkBox.ReadOnly = true;
            this._bookmarkBox.Size = new System.Drawing.Size(220, 19);
            this._bookmarkBox.TabIndex = 10;
            // 
            // status1
            // 
            this.status1.AutoSize = true;
            this.status1.Location = new System.Drawing.Point(44, 124);
            this.status1.Name = "status1";
            this.status1.Size = new System.Drawing.Size(0, 12);
            this.status1.TabIndex = 12;
            // 
            // ExportDialog
            // 
            this.AcceptButton = this._okButton;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
            this.CancelButton = this._cancelButton;
            this.ClientSize = new System.Drawing.Size(382, 150);
            this.ControlBox = false;
            this.Controls.Add(this.status1);
            this.Controls.Add(this._selectBookmarkButton);
            this.Controls.Add(this._bookmarkBox);
            this.Controls.Add(this._selectDirectory);
            this.Controls.Add(this._directoryBox);
            this.Controls.Add(this._lDirectory);
            this.Controls.Add(this._lBookmark);
            this.Controls.Add(this._cancelButton);
            this.Controls.Add(this._okButton);
            this.Controls.Add(this._targetBox);
            this.Controls.Add(this._lTarget);
            this.Controls.Add(this._formatBox);
            this.Controls.Add(this._lFormat);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MinimizeBox = false;
            this.Name = "ExportDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "データのエクスポート";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private Label _lBookmark;
        private Label _lDirectory;
        private TextBox _directoryBox;
        private Button _selectDirectory;
        private Button _selectBookmarkButton;
        private TextBox _bookmarkBox;

        private const int CURRENT_BRAND = 0;
        private const int BOOKMARK_BRAND = 1;

// modified by masta 20070311
        private const int ALL_BRAND = 2;
// modified by masta 20070311

        private void InitUI()
        {
            _targetBox.Items.Add(String.Format("現在表示している銘柄({0})", Env.Frame.ChartCanvas.GetBrand().Name));
            _targetBox.Items.Add("指定したブックマークのフォルダ内の銘柄");
// modified by masta 20070311
            _targetBox.Items.Add("全部の上場銘柄");
// modified by masta 20070311
            _targetBox.SelectedIndex = 0;
            _formatBox.SelectedIndex = 0;
        }

        private void OnOK(object sender, EventArgs args) {
            this.DialogResult = DialogResult.None;
            ExportFormat fmt = _formatBox.SelectedIndex==0? ExportFormat.CSV : ExportFormat.XML;
            string directory = _directoryBox.Text;
            if(directory.Length == 0) {
                Util.Warning("ディレクトリが指定されていません。");
                return;
            }

            if(!Directory.Exists(directory)) {
                if(Util.AskUserYesNo(this, String.Format("{0}は存在しません。作成しますか？", directory)) == DialogResult.Yes) {
                    try {
                        Directory.CreateDirectory(directory);
                    } catch(Exception) {
                        Util.Warning("作成できませんでした。");
                        return;
                    }
                } else
                    return;
            }

            //Export Directory
            try {
                if (_targetBox.SelectedIndex == CURRENT_BRAND)
                {
                    BasicBrand br = Env.Frame.ChartCanvas.GetBrand() as BasicBrand;
                    if (br == null)
                    {
                        Util.Warning("派生銘柄をエクスポートはできません。");
                        return;
                    }
                    Export(directory, br, fmt);
                }
                else if (_targetBox.SelectedIndex == BOOKMARK_BRAND) // modified by masta 20070311
                {
                    if (_bookmarkFolder == null)
                    {
                        Util.Warning("ブックマークフォルダが指定されていません。");
                        return;
                    }
                    BookmarkedBrandEnumerator en = new BookmarkedBrandEnumerator(_bookmarkFolder);
                    AbstractBrand br = en.Next();
                    while (br != null)
                    {
                        if (br is BasicBrand)
                            Export(directory, br as BasicBrand, fmt);
                        br = en.Next();
                    }
                }
                else
                { // modified by masta 20070311
                    // napo
                    System.Collections.IDictionaryEnumerator ie = Env.BrandCollection.GetEnumerator();
                    int total = Env.BrandCollection.TotalCount;
                    int now = 0;
                    while (ie.MoveNext())
                    {
                        now++;
                        if (ie.Value is AbstractBrand)
                        {
                            continue;
                        }
                        AbstractBrand br = ie.Value as AbstractBrand;
                        if (br is BasicBrand)
                        {
                            continue;
                        }
                        Export(directory, br as BasicBrand, fmt);
                        this.status1.Text = String.Format("{0}/{1}", now, total);
                    }
                    this.status1.Text = "";
                    // modified by masta 20070311
                }
                this.DialogResult = DialogResult.OK; //successfully exit
            } catch(Exception ex) {
                Util.ReportCriticalError(ex);
            }
        }

        private void OnSelectDirectory(object sender, EventArgs args) {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.Description = "データを保存するディレクトリを指定してください。";
            dlg.ShowNewFolderButton = true;
            if(dlg.ShowDialog(this)==DialogResult.OK)
                _directoryBox.Text = dlg.SelectedPath;
        }

        private void OnSelectBookmarkFolder(object sender, System.EventArgs e) {
            SelectBookmarkFolder dlg = new SelectBookmarkFolder();
			dlg.ShowCreateFolderButton = false;
            if(dlg.ShowDialog(this) == DialogResult.OK) {
                _bookmarkFolder = dlg.ResultFolder;
                _bookmarkBox.Text = _bookmarkFolder.Name;
            }
        }
        private void OnTargetChanged(object sender, EventArgs args) {
            bool e = _targetBox.SelectedIndex == BOOKMARK_BRAND;
            _bookmarkBox.Enabled = e;
            _selectBookmarkButton.Enabled = e;
        }

        private void Export(string directory, BasicBrand br, ExportFormat fmt) {
            string filename = String.Format("{0}\\{1}.{2}", directory, br.CodeAsString, fmt==ExportFormat.XML? "xml" : "csv");
            DataFarm farm = Env.BrandCollection.CreateDailyFarm(br, 0);
            if(!farm.IsEmpty)
                DataExporter.ExportData(filename, farm, fmt);
        }

        private Label status1;


    }
}