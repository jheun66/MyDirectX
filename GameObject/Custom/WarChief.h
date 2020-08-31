#pragma once

class WarChief : public Model
{
public:
	WarChief();
	~WarChief();

	void Render() override;

private:
	MSBuffer* settingBuffer;
};