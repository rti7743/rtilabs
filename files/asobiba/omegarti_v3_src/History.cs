using System;
using System.Collections;
using System.Diagnostics;

using Zanetti.Data;

namespace Zanetti.Commands
{
	//ブラウザ風の進む・戻るの履歴を表現する Undo/Redoの履歴ともいえる
	internal class BrandHistory {
		private ArrayList _back;
		private ArrayList _front;
		private const int LIMIT_COUNT = 20;

		public BrandHistory() {
			_back = new ArrayList();
			_front = new ArrayList();
		}
		public bool HasBack {
			get {
				return _back.Count>0;
			}
		}
		public bool HasFront {
			get {
				return _front.Count>0;
			}
		}
		public IEnumerable BackBrands {
			get {
				return _back;
			}
		}
		public IEnumerable FrontBrands {
			get {
				return _front;
			}
		}

		//銘柄を表示
		public void Update(AbstractBrand br) {
			AbstractBrand top = _back.Count==0? null : (AbstractBrand)_back[0];
			if(top==br) return; //同一銘柄を表示しても更新しない
			_back.Insert(0, br); //やや非効率だが個数は少ないのでOKとする
			
			if(_back.Count>LIMIT_COUNT) _back.RemoveAt(_back.Count-1);
			_front.Clear(); //Redoバッファはクリアされる
		}

		//step個戻った銘柄を返す。普通は１だが、メニューから選択したときは２以上のこともある
		public AbstractBrand Back(AbstractBrand current, int step) {
			for(int i=0; i<step; i++) {
				Debug.Assert(_back.Count>0);
				_front.Insert(0, current);
				current = (AbstractBrand)_back[0];
				_back.RemoveAt(0);
			}

			while(_front.Count>LIMIT_COUNT) _front.RemoveAt(_back.Count-1);
			return current;
		}
		public AbstractBrand Redo(AbstractBrand current, int step) {
			for(int i=0; i<step; i++) {
				Debug.Assert(_front.Count>0);
				_back.Insert(0, current);
				current = (AbstractBrand)_front[0];
				_front.RemoveAt(0);
			}

			while(_back.Count>LIMIT_COUNT) _back.RemoveAt(_back.Count-1);
			return current;
		}
	}
}
