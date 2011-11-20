using System;
using System.Text;
using System.Diagnostics;

using Zanetti.Data;

namespace Zanetti.UI {
    //価格帯別出来高 考えたところ拡張キットで統一的に扱うのは厳しい。今後の課題だ。
    internal class AccumulativeVolume {
        public const int DATA_PER_SCALELINE = 5; //１つのPriceScaleで何本書くか

        private double[] _data;
        private int _beginDate;
        private int _endDate;
        private int _dataLength; //毎回配列をnewはしないのでこれを長さとして扱う
        private double _startPrice; //集計する値段の開始値
        private double _pitch;      //刻み値
        private double _endPrice;
        private double _maxVolume;  //保持している最大の出来高
        private bool _available;


        public AccumulativeVolume() {

        }

        public int DataLength {
            get {
                return _dataLength;
            }
        }
        public double this[int index] {
            get {
                return _data[index];
            }
        }

        public double StartPrice {
            get {
                return _startPrice;
            }
        }
        public double EndPrice {
            get {
                return _endPrice;
            }
        }

        public double Pitch {
            get {
                return _pitch;
            }
        }
        public double MaxVolume {
            get {
                return _maxVolume;
            }
        }
        public int BeginDate {
            get {
                return _beginDate;
            }
        }
        public int EndDate {
            get {
                return _endDate;
            }
        }


        public bool Available {
            get {
                return _available;
            }
            set {
                _available = value;
            }
        }

        private void SetLength(int length) {
            if(_data == null || _data.Length < length)
                _data = new double[length];
            _dataLength = length;
            for(int i = 0; i < length; i++) _data[i] = 0;
        }

        public void Fill(DataFarm farm, int target_index, double start_price, double pitch, double end_price) {
			//一目均衡表など未来の領域を表示している場合
			if(target_index >= farm.FilledLength || !farm.Brand.IsVolumeAvailable) {
				_available = false;
				return;
			}

            start_price = pitch * Math.Floor(start_price / pitch);

            SetLength((int)((end_price - start_price) / pitch));
			if(_dataLength==0) return;
            _startPrice = start_price;
            _endPrice = start_price + pitch * _dataLength;
            _pitch = pitch;
            _maxVolume = 0;
            _available = true; //データを埋めた場合は常に有効


            //長さの調整は別途オプションから取ってくる
            int start_index = Math.Max(0, target_index - Env.Preference.AccumulativeVolumePeriod.GetValue(Env.Options.ChartFormat));
            _beginDate = farm.GetByIndex(start_index).Date;
            _endDate = farm.GetByIndex(target_index).Date;
            for(int index = start_index; index <= target_index; index++) {
                TradeData td = farm.GetByIndex(index);
                double volume = td.Volume / 4;
                //４本値に1/4ずつの出来高を加算
                AddVolume(td.Open, volume);
                AddVolume(td.High, volume);
                AddVolume(td.Low, volume);
                AddVolume(td.Close, volume);
            }
		}
        private void AddVolume(double value, double volume) {
            int t = (int)Math.Floor((value - _startPrice) / _pitch);
            if(t >= 0 && t < _dataLength) {
                _data[t] += volume;
                _maxVolume = Math.Max(_maxVolume, _data[t]);
            }
        }
    }
}
