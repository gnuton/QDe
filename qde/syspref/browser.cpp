#include "browser.h"

Browser::Browser(QWidget *parent)
	: QWidget(parent)
{
	layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}

Browser::~Browser()
{
}

void Browser::addSection(AmeStaticIconList *section)
{
	layout->addWidget(section);
}

void Browser::addStretch()
{
	layout->addStretch();
}
