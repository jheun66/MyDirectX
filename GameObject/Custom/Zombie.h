#pragma once

class Zombie : public Model
{
public:
	Zombie();
	~Zombie();

	void Render() override;

private:
	MSBuffer* settingBuffer;
};