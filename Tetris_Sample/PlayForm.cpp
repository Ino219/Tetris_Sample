#include "PlayForm.h"

using namespace TetrisSample;
using namespace System::Drawing::Drawing2D;
using namespace System;
using namespace System::Collections::Generic;


[STAThreadAttribute]

int main() {
	Application::Run(gcnew PlayForm());
	return 0;
}


System::Void TetrisSample::PlayForm::PlayForm_Load(System::Object ^ sender, System::EventArgs ^ e)
{
	SetField();
	createGrid();
	createBlock(1, gcnew Point(3,0), 0);
	FallTimer->Enabled = true;
	ctrlFlag = true;
	this->KeyPreview = true;
	//this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &PlayForm::keyControl);
	return System::Void();
}

System::Void TetrisSample::PlayForm::SetField()
{
	//�t�B�[���h�̃O���b�h��`�ƃ��C���쐬
	GridX = Field->Width / line_space;
	GridY = Field->Height / line_space;
	arr_GridX = GridX-1;
	arr_GridY = GridY-1;
	for (int i = 1; i < GridX; i++) {
		gr->DrawLine(pn, i*line_space, 0,i*line_space, Field->Height);
	}
	for (int j = 0; j < GridY; j++) {
		gr->DrawLine(pn,  0, j*line_space,  Field->Width,j*line_space);
	}
	Field->Refresh();
	return System::Void();
}

System::Void TetrisSample::PlayForm::createGrid()
{
	//�t�B�[���h�̃O���b�h�z����쐬���A������
	FieldGrid = gcnew array<array<bool>^>(GridY);
	for (int i = 0; i < GridY; i++) {
		FieldGrid[i] = gcnew array<bool>(GridX);
		for (int j = 0; j < GridX; j++) {
			FieldGrid[i][j] = false;
		}
	}
	return System::Void();
}

System::Void TetrisSample::PlayForm::createBlock(int pattern, Point ^ position, int dir)
{
	nowDir = dir;
	mb = gcnew block_pattern();
	mb = return_Block(pattern, position);
	array<Point^>^ tList = mb->setInit();
	for each (Point^ var in tList)
	{
		drawBox(var,blockFlame,blockColor);
	}
	return System::Void();
}

PlayForm::block_pattern^ TetrisSample::PlayForm::return_Block(int pattern,Point^ position)
{
	//�O���b�h���W�Ŏw��
	block_pattern^ temp_bp = gcnew block_pattern();
	if (pattern == 1) {
		temp_bp->p1 = position;
		temp_bp->p2 = gcnew Point(position->X, position->Y + 1);
		temp_bp->p3 = gcnew Point(position->X+1,position->Y+1);
		temp_bp->p4 = gcnew Point(position->X + 2, position->Y + 1);
	}
	else if (pattern == 2) {
		temp_bp->p1 = position;
		temp_bp->p2 = gcnew Point(position->X,position->Y+1);
		temp_bp->p3 = gcnew Point(position->X+1, position->Y+1);
		temp_bp->p4 = gcnew Point(position->X+1, position->Y+2);
	}
	else if (pattern == 3) {
		temp_bp->p1 = position;
		temp_bp->p2 = gcnew Point(position->X, position->Y + 1);
		temp_bp->p3 = gcnew Point(position->X-1, position->Y + 1);
		temp_bp->p4 = gcnew Point(position->X - 1, position->Y + 2);
	}
	else if (pattern == 4) {
		temp_bp->p1 = position;
		temp_bp->p2 = gcnew Point(position->X, position->Y + 1);
		temp_bp->p3 = gcnew Point(position->X+1, position->Y);
		temp_bp->p4 = gcnew Point(position->X+1, position->Y + 1);

	}
	nowPattern = pattern;
	return temp_bp;
}

System::Void TetrisSample::PlayForm::drawBox(Point ^ position,Color flame,Color inside)
{
	//position�̓O���b�h���W
	int x = position->X*line_space;
	int y = position->Y*line_space;
	gr->FillRectangle((gcnew SolidBrush(inside)), x, y, blockXY, blockXY);
	gr->DrawRectangle((gcnew Pen(flame)), x, y, blockXY, blockXY);

	FieldGrid[position->Y][position->X] = true;
	Field->Refresh();
	
	return System::Void();
}

System::Void TetrisSample::PlayForm::FallTimer_Tick(System::Object ^ sender, System::EventArgs ^ e)
{
	MoveBlock(3);
	return System::Void();
}

System::Void TetrisSample::PlayForm::MoveBlock(int dir)
{
	array<Point^>^ tempArray = mb->setInit();
	edge^ temp = EdgeCheck(tempArray);
	if ((dir == 1&&temp->left)|| (dir == 2 && temp->right)) {
		return;
	}
	else if (dir == 3 && temp->down) {
		for each (Point^ var in tempArray)
		{
			if (RowDeleteCheck(var)) {
				Delete_Row(var->Y);
			}
		}
		RepopBlock(gcnew Point(3, 0));
		return;
	}
	if (HitboxCheck(tempArray, dir)) {
		if (dir == 3) {
			for each (Point^ var in tempArray)
			{
				if (RowDeleteCheck(var)) {
					Delete_Row(var->Y);
				}
			}
			RepopBlock(gcnew Point(3, 0));
		}
		return;
	}

	Delete_mb(tempArray);

	if (dir == 1) {
		//��
			for each (Point^ var in tempArray)
			{
				var->X -= 1;
			}
	}

	if (dir == 2) {
		//�E
			for each (Point^ var in tempArray)
			{
				var->X += 1;
			}
	}

	if (dir == 3) {
		//��
			for each (Point^ var in tempArray)
			{
				var->Y += 1;
			}
	}

	mb->set(tempArray);
	drawBlock(mb->pList);

	return System::Void();
}

System::Void TetrisSample::PlayForm::drawBlock(array<Point^>^ arr)
{
	for each (Point^ var in arr)
	{
		drawBox(var, blockFlame, blockColor);
	}
	Field->Refresh();
	return System::Void();
}
/*
System::Void TetrisSample::PlayForm::keyControl(System::Object ^ sender, System::Windows::Forms::KeyEventArgs ^ e)
{
	//�W���������͂̏����͂������
	
	if (e->KeyCode == Keys::A) {
		//MessageBox::Show("A����");
	}
	return System::Void();
}
*/
bool TetrisSample::PlayForm::ProcessDialogKey(Keys keyData)
{
	if (!ctrlFlag) {
		return false;
	}
	//�L�[������`
	//���̃��\�b�h�̓v���v���Z�X���ɌĂяo�����̂ŁA������I�[�o�[���C�h����
	switch (keyData)
	{
	case Keys::Down:
		MoveBlock(3);
		break;
	case Keys::Right:
		MoveBlock(2);
		break;
	case Keys::Up:
		break;
	case Keys::Left:
		MoveBlock(1);
		break;
	case Keys::Enter:
		//��]����
		mb = SetRotatePattern(nowPattern, mb->p1);
		mb->setInit();
		drawBlock(mb->pList); 
		
	default:
		return true;
		//return ProcessDialogKey(keyData);
		//return IsInputKey(keyData);
		break;
	}
	return true;
}

PlayForm::block_pattern ^ TetrisSample::PlayForm::SetRotatePattern(int pattern, Point^ position)
{
	array<Point^>^ tempArray = mb->setInit();
	edge^ tempEdge = EdgeCheck(tempArray);
	Delete_mb(tempArray);

	//�����ϐ��̏C��
	int dir = nowDir;
	if (dir < 3) {
		dir += 1;
	}
	else {
		dir = 0;
	}

	//���_
	Point^ O = position;
	block_pattern^ temp_bp = gcnew block_pattern();

		if (pattern == 1) {
			if (dir == 0) {
				//�f�t�H���g
				temp_bp->p1 = O;
				temp_bp->p2 = gcnew Point(O->X, O->Y + 1);
				temp_bp->p3 = gcnew Point(O->X + 1, O->Y + 1);
				temp_bp->p4 = gcnew Point(O->X + 2, O->Y + 1);
			}
			else if (dir == 1) {
				//�E��]90
				temp_bp->p1 = gcnew Point(O->X, O->Y + 2);
				temp_bp->p2 = gcnew Point(O->X + 1, O->Y + 2);
				temp_bp->p3 = gcnew Point(O->X + 1, O->Y + 1);
				temp_bp->p4 = gcnew Point(O->X + 1, O->Y);
			}
			else if (dir == 2) {
				temp_bp->p1 = gcnew Point(O->X+2, O->Y);
				temp_bp->p2 = gcnew Point(O->X + 2, O->Y - 1);
				temp_bp->p3 = gcnew Point(O->X + 1, O->Y - 1);
				temp_bp->p4 = gcnew Point(O->X , O->Y-1);
			}
			else if (dir == 3) {
				temp_bp->p1 = gcnew Point(O->X-2, O->Y-2);
				temp_bp->p2 = gcnew Point(O->X-1, O->Y-2);
				temp_bp->p3 = gcnew Point(O->X-2, O->Y - 1);
				temp_bp->p4 = gcnew Point(O->X-2, O->Y);
			}
		}
		else if (pattern == 2) {
			if (dir == 0||dir==2) {
				//�f�t�H���g
				temp_bp->p1 = O;
				temp_bp->p2 = gcnew Point(O->X, O->Y + 1);
				temp_bp->p3 = gcnew Point(O->X + 1, O->Y + 1);
				temp_bp->p4 = gcnew Point(O->X + 1, O->Y + 2);
			}
			else if (dir == 1||dir==3) {
				if (tempEdge->left) {
					O->X += 1;
				}
				//�E��]90
				temp_bp->p1 = gcnew Point(O->X, O->Y);
				temp_bp->p2 = gcnew Point(O->X+1, O->Y );
				temp_bp->p3 = gcnew Point(O->X, O->Y+1);
				temp_bp->p4 = gcnew Point(O->X - 1, O->Y+1);
			}
		}
		else if (pattern == 3) {
			if (dir == 0 || dir == 2) {
				temp_bp->p1 = O;
				temp_bp->p2 = gcnew Point(O->X, O->Y + 1);
				temp_bp->p3 = gcnew Point(O->X - 1, O->Y + 1);
				temp_bp->p4 = gcnew Point(O->X - 1, O->Y + 2);
			}
			else if (dir == 1 || dir == 3) {
				temp_bp->p1 = O;
				temp_bp->p2 = gcnew Point(O->X-1, O->Y);
				temp_bp->p3 = gcnew Point(O->X, O->Y + 1);
				temp_bp->p4 = gcnew Point(O->X + 1, O->Y + 1);
			}
		}
		else if (pattern == 4) {
			//return;
		}

		edge^ re = RotateEdgeCheck(temp_bp);
		if (re->left) {
			temp_bp=SlideBlock(temp_bp, 1, re->lb);
		}
		if (re->right) {
			temp_bp = SlideBlock(temp_bp, 2, re->rb);
		}
		if (re->down) {
			temp_bp = SlideBlock(temp_bp, 3, re->db);
		}
		if (re->up) {
			temp_bp = SlideBlock(temp_bp, 4, re->ub);
		}

		nowDir = dir;
	
	//throw gcnew System::NotImplementedException();
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return temp_bp;
}

System::Void TetrisSample::PlayForm::Delete_mb(array<Point^>^ arr)
{
	for each (Point^ var in arr)
	{
		drawBox(var, baseFlame, baseColor);
		FieldGrid[var->Y][var->X] = false;
	}
	Field->Refresh();
	return System::Void();
}

PlayForm::edge ^ TetrisSample::PlayForm::EdgeCheck(array<Point^>^ arr)
{
	edge^ temp = gcnew edge;
	bool left=false;
	bool right=false;
	bool down=false;

	for each (Point^ var in arr)
	{
		if (var->X <= 0) {
			left = true;
		}
		if (var->X >= arr_GridX) {
			right = true;
		}
		if (var->Y == arr_GridY) {
			down = true;
		}
	}

	temp->left = left;
	temp->right = right;
	temp->down = down;

	//throw gcnew System::NotImplementedException();
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�

	return temp;
}

bool TetrisSample::PlayForm::HitboxCheck(array<Point^>^ arr, int dir)
{
	bool check=false;

	for each (Point^ var in arr)
	{
		if (dir == 1) {
			//��
			Point^ leftX=gcnew Point(var->X - 1,var->Y);
			if (MatchCheck(arr, leftX)) {
				continue;
			}
			if (FieldGrid[leftX->Y][leftX->X]) {
				check = true;
			}
			

		}
		else if (dir == 2) {
			//�E
			Point^ rightX = gcnew Point(var->X + 1,var->Y);
			if (MatchCheck(arr, rightX)) {
				continue;
			}
			if (FieldGrid[rightX->Y][rightX->X]) {
				check = true;
			}
			
		}
		else if (dir == 3) {
			//��
			Point^ downY = gcnew Point(var->X,var->Y + 1);
			if (MatchCheck(arr, downY)) {
				continue;
			}
			if (FieldGrid[downY->Y][downY->X]) {
				check = true;
			}
			
		}
	}
	return check;
}

bool TetrisSample::PlayForm::MatchCheck(array<Point^>^ arr, Point^ value)
{
	//value�͈ړ�����W
	bool check=false;
	for each (Point^ var in arr)
	{
		if (var->Equals(value)) {
			check = true;
		}
	}
	return check;
}

bool TetrisSample::PlayForm::RowDeleteCheck(Point^ p)
{
	bool rowcheck = true;
	for (int i = 0; i < GridX; i++) {
		if (!FieldGrid[p->Y][i]) {
			rowcheck = false;
		}
	}
	return rowcheck;
}

System::Void TetrisSample::PlayForm::Delete_Row(int y)
{
	for (int i = 0; i < GridX; i++) {
		Point^ temp = gcnew Point(i, y);
		drawBox(temp, baseFlame, baseColor);
		FieldGrid[y][i] = false;
	}

	for (int j = y; j >= 0; j--) {
		for (int n = 0; n < GridX; n++) {
			//�f���[�g�����Ƀu���b�N������ꍇ
			if (FieldGrid[j][n]) {
				Point^ p = gcnew Point(n, j+1);
				if (!CheckDownPosition(p)) {
					//����Ƀu���b�N���Ȃ��ꍇ
					//�ړ���̍��W�ɃX���C�h����
					SlideDownBlock(p);
				}
			}
		}
	}

	Field->Refresh();
	return System::Void();
}

bool TetrisSample::PlayForm::CheckDownPosition(Point ^ position)
{
	bool check = false;
	if (FieldGrid[position->Y][position->X]) {
		check = true;
	}
	return check;
}

System::Void TetrisSample::PlayForm::SlideDownBlock(Point ^ position)
{
	//position�͈ړ�����W
	Point^ b_position = gcnew Point(position->X,position->Y-1);
	drawBox(b_position, baseFlame, baseColor);
	FieldGrid[b_position->Y][b_position->X] = false;

	drawBox(position, blockFlame, blockColor);

	Field->Refresh();

	return System::Void();
}

PlayForm::edge ^ TetrisSample::PlayForm::RotateEdgeCheck(block_pattern ^ bp)
{
	edge^ res = gcnew edge;
	array<Point^>^ temp = bp->setInit();
	for each (Point^ var in temp)
	{
		if (var->X < 0) {
			res->left = true;
			res->lb = Math::Abs(var->X - 0);
		}
		if (var->X > arr_GridX) {
			res->right = true;
			res->rb = var->X - arr_GridX;
		}
		if (var->Y > arr_GridY) {
			res->down = true;
			res->db = var->Y - arr_GridY;
		}
		if (var->Y < 0) {
			res->up = true;
			res->ub = Math::Abs(var->Y - 0);
		}
	}

	//throw gcnew System::NotImplementedException();
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return res;
}

PlayForm::block_pattern^ TetrisSample::PlayForm::SlideBlock(block_pattern ^ st, int dir, int num)
{
	array<Point^>^ temp = st->setInit();
	for each (Point^ var in temp)
	{
		if (dir == 1) {
			//��
			var->X += num;
		}
		else if (dir==2) {
			//�E
			var->X -= num;
		}
		else if (dir == 3) {
			//��
			var->Y -= num;
		}
		else if (dir == 4) {
			//��
			var->Y += num;
		}
		
	}
	st->set(temp);

	return st;
}

System::Void TetrisSample::PlayForm::RepopBlock(Point ^ position)
{
	Random^ rnd = gcnew Random();
	int bp = rnd->Next(1, 4);
	Random^ rnd2 = gcnew Random();
	int dir = rnd2->Next(1, 4);
	if (!RepopCheck(bp, position)) {
		createBlock(bp, position, dir);
	}
	else {
		FallTimer->Enabled = false;
		ctrlFlag = false;
		MessageBox::Show("GameOver");
	}
	return System::Void();
}

bool TetrisSample::PlayForm::RepopCheck(int pattern, Point ^ position)
{
	bool check = false;

	block_pattern^ temp  = gcnew block_pattern();
	temp = return_Block(pattern, position);
	array<Point^>^ tList = temp->setInit();
	for each (Point^ var in tList)
	{
		if (FieldGrid[var->Y][var->X]) {
			check = true;
		}
	}
	return check;
}


