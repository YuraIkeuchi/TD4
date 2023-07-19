#include "Menu.h"

void (Menu::* Menu::stateTable[])() = {
	&Menu::NoneUpdate,
	&Menu::OpenUpdate,
	&Menu::SelectUpdate,
	&Menu::CheckUpdate,
	&Menu::CloseUpdate,

};

Menu::Menu() {


	sprites_.push_back(

}

Menu::~Menu() {

}

void Menu::Initialize() {
}

void Menu::Update() {
}

void Menu::Draw() {


}

void Menu::NoneUpdate() {
}

void Menu::OpenUpdate() {
}

void Menu::SelectUpdate() {
}

void Menu::CheckUpdate() {
}

void Menu::ConfirmUpdate() {
}

void Menu::CloseUpdate() {
}

std::unique_ptr<IKESprite> Menu::CreateSprite(UINT num) {
	std::unique_ptr<IKESprite>
	return ;
}
