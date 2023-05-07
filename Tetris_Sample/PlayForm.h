#pragma once

namespace TetrisSample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// PlayForm の概要
	/// </summary>
	public ref class PlayForm : public System::Windows::Forms::Form
	{
	public:
		PlayForm(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクター コードを追加します
			//
			bmp = gcnew Bitmap(Field->Width, Field->Height);
			Field->Image = bmp;
			gr = Graphics::FromImage(Field->Image);
			pn = gcnew Pen(Color::Gray, 1);

			line_space = 25;
			blockXY = 25;

			_affine = gcnew System::Drawing::Drawing2D::Matrix();
			ctrlFlag = false;
			nowPattern = 0;
			nowDir = 0;
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~PlayForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  Field;
	private: System::Windows::Forms::Timer^  FallTimer;
	private: System::Windows::Forms::Button^  button1;
	private: System::ComponentModel::IContainer^  components;
	protected:

	protected:

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Field = (gcnew System::Windows::Forms::PictureBox());
			this->FallTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Field))->BeginInit();
			this->SuspendLayout();
			// 
			// Field
			// 
			this->Field->BackColor = System::Drawing::Color::MistyRose;
			this->Field->Location = System::Drawing::Point(13, 13);
			this->Field->Name = L"Field";
			this->Field->Size = System::Drawing::Size(200, 250);
			this->Field->TabIndex = 0;
			this->Field->TabStop = false;
			// 
			// FallTimer
			// 
			this->FallTimer->Interval = 500;
			this->FallTimer->Tick += gcnew System::EventHandler(this, &PlayForm::FallTimer_Tick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(220, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &PlayForm::button1_Click);
			// 
			// PlayForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(307, 290);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->Field);
			this->Name = L"PlayForm";
			this->Text = L"PlayForm";
			this->Load += gcnew System::EventHandler(this, &PlayForm::PlayForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Field))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		Bitmap^ bmp;
		Graphics^ gr;
		Pen^ pn;
		//線幅を定義
		int line_space;
		//ブロック辺
		int blockXY;
		//縦グリッド
		int GridY;
		int arr_GridY;
		//横グリッド
		int GridX;
		int arr_GridX;
		array<array<bool>^>^ FieldGrid;
	public:
		ref struct block_pattern {
			//四つのポイントを保持
			Point^ p1;
			Point^ p2;
			Point^ p3;
			Point^ p4;
			array<Point^>^ pList;

			array<Point^>^ setInit() {
				pList = gcnew array<Point^>(4);
				pList[0] = p1;
				pList[1] = p2;
				pList[2] = p3;
				pList[3] = p4;
				return pList;
			}
			void set(array<Point^>^ arrays) {
				p1 = arrays[0];
				p2 = arrays[1];
				p3 = arrays[2];
				p4 = arrays[3];
			}
		};
		ref struct rotate_pattern {
			block_pattern^ bp0;
			block_pattern^ bp1;
			block_pattern^ bp2;
			block_pattern^ bp3;
		};
		ref struct edge {
			//接しているなら、どの端に接しているか
			bool left;
			bool right;
			bool down;
			bool up;
			//はみだし処理用
			int lb;
			int rb;
			int db;
			int ub;
		};
		block_pattern^ mb;
		Color baseColor = Color::MistyRose;
		Color blockColor = Color::LemonChiffon;
		Color blockFlame = Color::Blue;
		Color baseFlame = Color::Gray;
		bool ctrlFlag;
		int nowPattern;
		int nowDir;
		//アフィン行列
		Drawing2D::Matrix^ _affine;
		
		
	private: System::Void PlayForm_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void SetField();
	private: System::Void createGrid();
	//ブロックの種類、座標、向きを指定
	private: System::Void createBlock(int pattern,Point^ position,int dir);
	private: block_pattern^ return_Block(int pattern,Point^ position);
	private: System::Void drawBox(Point^ position,Color flame,Color base);
	private: System::Void FallTimer_Tick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MoveBlock(int dir);
	private: System::Void drawBlock(array<Point^>^ arr);
	//private: System::Void keyControl(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	protected: virtual bool ProcessDialogKey(Keys keyData) override;
	private: block_pattern^ SetRotatePattern(int pattern,Point^ position);
	private: System::Void Delete_mb(array<Point^>^ arr);
	private: edge^ EdgeCheck(array<Point^>^ arr);
	private: bool HitboxCheck(array<Point^>^ arr,int dir);
	private: bool MatchCheck(array<Point^>^ arr,Point^ value);
	private: bool RowDeleteCheck(Point^ p);
	private: System::Void Delete_Row(int y);
	private: bool CheckDownPosition(Point^ position);
	private: System::Void SlideDownBlock(Point^ position);
	private: edge^ RotateEdgeCheck(block_pattern^ bp);
	private: block_pattern^ SlideBlock(block_pattern^ st, int dir, int num);
	private: System::Void RepopBlock(Point^ position);
	private: bool RepopCheck(int pattern,Point^ position);

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	if (ctrlFlag) {
		MoveBlock(1);
	}
}
};
}
