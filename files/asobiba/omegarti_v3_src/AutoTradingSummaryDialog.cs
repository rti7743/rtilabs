using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using Zanetti.SystemTrading.AutoTrading;
using Zanetti.UI;
//hacked by rti
using Zanetti.SystemTrading.MoneyPlanning;
//-----

namespace Zanetti.Forms
{
	/// <summary>
	/// AutoTradingSummaryDialog の概要の説明です。
	/// </summary>
	internal class AutoTradingSummaryDialog : System.Windows.Forms.Form
	{
        //hacked by rti
        Hashtable HelpMessage;
        //最後に選択していたリストビューのアイテム
        int LastListViewItem;
        //-----------

		private System.Windows.Forms.Button _copyButton;
        private System.Windows.Forms.Button _okButton;
        private ToolTip toolTip1;
        private TabPage tabPage2;
        private TextBox SimLogTextBox;
        private TabPage tabPage1;
        private ListView _listView;
        private ColumnHeader _nameColumn;
        private ColumnHeader _valueColumn;
        private TabControl tabControl1;
        private IContainer components;

		public AutoTradingSummaryDialog(AutoTradingResult sr)
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
			InitUI(sr);
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this._copyButton = new System.Windows.Forms.Button();
            this._okButton = new System.Windows.Forms.Button();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.SimLogTextBox = new System.Windows.Forms.TextBox();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this._listView = new System.Windows.Forms.ListView();
            this._nameColumn = new System.Windows.Forms.ColumnHeader();
            this._valueColumn = new System.Windows.Forms.ColumnHeader();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage2.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.SuspendLayout();
            // 
            // _copyButton
            // 
            this._copyButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._copyButton.Location = new System.Drawing.Point(16, 458);
            this._copyButton.Name = "_copyButton";
            this._copyButton.Size = new System.Drawing.Size(75, 23);
            this._copyButton.TabIndex = 2;
            this._copyButton.Text = "コピー";
            this._copyButton.Click += new System.EventHandler(this.OnCopyButton);
            // 
            // _okButton
            // 
            this._okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this._okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this._okButton.Location = new System.Drawing.Point(288, 458);
            this._okButton.Name = "_okButton";
            this._okButton.Size = new System.Drawing.Size(75, 23);
            this._okButton.TabIndex = 0;
            this._okButton.Text = "OK";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.SimLogTextBox);
            this.tabPage2.Location = new System.Drawing.Point(4, 21);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(359, 418);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "売買ログ";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // SimLogTextBox
            // 
            this.SimLogTextBox.Location = new System.Drawing.Point(4, 5);
            this.SimLogTextBox.Multiline = true;
            this.SimLogTextBox.Name = "SimLogTextBox";
            this.SimLogTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.SimLogTextBox.Size = new System.Drawing.Size(355, 411);
            this.SimLogTextBox.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this._listView);
            this.tabPage1.Location = new System.Drawing.Point(4, 21);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(359, 418);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "結果";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // _listView
            // 
            this._listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this._nameColumn,
            this._valueColumn});
            this._listView.GridLines = true;
            this._listView.Location = new System.Drawing.Point(4, 5);
            this._listView.MultiSelect = false;
            this._listView.Name = "_listView";
            this._listView.Size = new System.Drawing.Size(353, 411);
            this._listView.TabIndex = 1;
            this._listView.UseCompatibleStateImageBehavior = false;
            this._listView.View = System.Windows.Forms.View.Details;
            // 
            // _nameColumn
            // 
            this._nameColumn.Text = "項目";
            this._nameColumn.Width = 160;
            // 
            // _valueColumn
            // 
            this._valueColumn.Text = "値";
            this._valueColumn.Width = 208;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(2, 10);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(367, 443);
            this.tabControl1.TabIndex = 4;
            // 
            // AutoTradingSummaryDialog
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
            this.ClientSize = new System.Drawing.Size(381, 490);
            this.ControlBox = false;
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this._okButton);
            this.Controls.Add(this._copyButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AutoTradingSummaryDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "自動売買の集計";
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion

		private void InitUI(AutoTradingResult sr) {
            this.HelpMessage = new Hashtable();

			AutoTradingResultSummary sum = sr.CreateSummary();

            AddItem("シグナル名", sr.Item.Title);
            AddItem("検証対象銘柄", sr.BrandDescription);
            //AddItem("シグナル発生時に使う価格", );
			AddItem("検証開始日", Util.FormatFullDate(sr.StartDate));
			AddItem("検証終了日", Util.FormatFullDate(sr.EndDate));
			AddItem("総検証日数", String.Format("{0}日", sr.TotalCheckCount));

			AddItem("シグナル発生数", String.Format("{0}回 (平均して{1:F2}日に１回)", sr.ResultCount, (double)sr.TotalCheckCount / sr.ResultCount));
            AddItem("１シグナルでの平均トレード日数", String.Format("{0:F2}日", sum._avgTradeLength));
            AddItem("１トレードの平均パフォーマンス", String.Format("{0:F2}%", sum._avgPerformance * 100));
			AddItem("パフォーマンスの標準偏差", String.Format("{0:F2}%", sum._sdTotal * 100));

			AddItem("成功数", String.Format("{0} ({1:F2}%)", sum._exitCount, (double)sum._exitCount / sum._totalCount * 100));
			AddItem("成功時の平均利益", String.Format("{0:F2}%", sum._avgExit * 100));
			AddItem("成功時の標準偏差", String.Format("{0:F2}%", sum._sdExit * 100));
			AddItem("成功時の平均トレード期間", String.Format("{0:F2}日", sum._avgExitTradeLength));
			SignalResult t = sum._maxExit;
			if(t!=null)
				AddItem("最大利益", String.Format("{0:F2}% ({1}{2} {3})", t.Performance(sr.Item.TradingType)*100, t.Brand.CodeAsString, t.Brand.Name, Util.FormatShortDate(t.StartDate)));
			else
				AddItem("最大利益", "-");

			AddItem("失敗数", String.Format("{0} ({1:F2}%)", sum._losscutCount, (double)sum._losscutCount / sum._totalCount * 100));
			AddItem("失敗時の平均損失", String.Format("{0:F2}%", sum._avgLosscut * 100));
			AddItem("失敗時の標準偏差", String.Format("{0:F2}%", sum._sdLosscut * 100));
			AddItem("失敗時の平均トレード期間", String.Format("{0:F2}日", sum._avgLosscutTradeLength));
			t = sum._maxLosscut;
			if(t!=null)
				AddItem("最大損失", String.Format("{0:F2}% ({1}{2} {3})", t.Performance(sr.Item.TradingType)*100, t.Brand.CodeAsString, t.Brand.Name, Util.FormatShortDate(t.StartDate)));
			else
				AddItem("最大損失", "-");

			AddItem("結果判定不能なシグナル数", (sr.ResultCount - sum._exitCount - sum._losscutCount).ToString());

            //haced by rti

            //実質売買を行う
            try
            {
                MoneyPlanningSimulator realsim = new MoneyPlanningSimulator(sr, 
                    Env.Preference.InitCach,  sr.MoneyPlanningItem);
                AddItem("実質売買シグナル名", sr.MoneyPlanningItem.Title);
                AddItem("初期資金", String.Format("{0:#,##0} 円", realsim.GetInitCash()));
                AddItem("最終資金", String.Format("{0:#,##0} 円 ({1:F2}%Up)", realsim.GetCash(), (((double)realsim.GetCash() / realsim.GetInitCash()) - 1) * 100));
                AddItem("実質勝利", String.Format("{0} 回 ({1:F2}%) 総計:{2:#,##0} 円", realsim.GetWinCount(), realsim.WinPercentage() , realsim.GetSouRieki()));
                AddItem("実質損失", String.Format("{0} 回 ({1:F2}%) 総計:{2:#,##0} 円", realsim.GetLoseCount(), realsim.LosePercentage(), realsim.GetSouSonshitu()));
                AddItem("残高不足", String.Format("{0} 回 ({1:F2}%)", realsim.GetNoMoneyCount(), (double)realsim.GetNoMoneyCount() / realsim.TotalCount() * 100));
                AddItem("最大ドローダウン", String.Format("{0:#,##0} 円", realsim.GetSaiteiRieki()));
                AddItem("プロフィットファクター", String.Format("{0:F2}", realsim.ProfitFactor()));
                AddItem("ペイオフレシオ(損益レシオ)", String.Format("{0:F2}", realsim.PayoffRatio()));
                AddItem("破産確率", String.Format("{0:F2}%", realsim.HasanKakuritu(realsim.PayoffRatio(), realsim.WinPercentage())));
                AddItem("資金曲線分析", String.Format("{0:F2}%", realsim.SikinKykusenBunseki()));
                this.SimLogTextBox.Text = realsim.GetLog();
            }
            catch(Exception e)
            {
                AddItem("実質売買エラー", e.Message);
            }
            //--------------
        }

		private void AddItem(string name, string value) {
			ListViewItem li = new ListViewItem(new string[] { name, value });
			_listView.Items.Add(li);
		}

		// Copyボタン
		// ListViewUtilを流用し、一応動作しているものの自信はなし by Yakutainashi
		private void OnCopyButton(object sender, EventArgs e) {
			ListViewUtil.CopyListViewContentToClipboard(_listView, new ListViewUtil.ItemChecker(ListViewItemChecker));
		}
		private static bool ListViewItemChecker(ListViewItem li) {
			return true;
		}

        private void moneylogButton_Click(object sender, EventArgs e)
        {
        }
	}
}
