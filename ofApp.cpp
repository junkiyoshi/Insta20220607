#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(2);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);

	for (int i = 0; i < 4; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}

	this->font_size = 35;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
	this->word = "2000Days";
}

//--------------------------------------------------------------
void ofApp::update() {

	int index = 0;
	auto radius = 200;
	ofColor color;
	for (auto& noise_seed : this->noise_seed_list) {

		auto deg = ofGetFrameNum() * 3 + index * 90;
		auto next_deg = deg + 3;

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));
	
		auto distance = location - next;
		distance *= 1;

		for (int i = 0; i < 2; i++) {

			auto future = location + distance * 8;
			auto random_deg = ofRandom(360);
			future += glm::vec2(15 * cos(random_deg * DEG_TO_RAD), 15 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));
			color.setHsb(ofRandom(255), 255, 200);
			this->color_list.push_back(color);
			this->word_index_list.push_back(ofRandom(this->word.size()));
		}

		index++;
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 1000) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->word_index_list.erase(this->word_index_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int index = 0;
	for (int i = 0; i < this->location_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->location_list[i]);

		auto angle = std::atan2(this->location_list[i].y, this->location_list[i].x);
		ofRotate(angle * RAD_TO_DEG);

		ofPath chara_path = this->font.getCharacterAsPoints(this->word[this->word_index_list[i]], true, false);
		vector<ofPolyline> outline = chara_path.getOutline();

		ofFill();
		ofSetColor(this->color_list[i], 128);
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			if (outline_index != 0) { ofNextContour(true); }

			auto vertices = outline[outline_index].getResampledByCount(100).getVertices();
			for (auto& vertex : vertices) {

				auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 0);
				ofVertex(location);
			}
		}
		ofEndShape(true);

		ofNoFill();
		ofSetColor(this->color_list[i]);
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			if (outline_index != 0) { ofNextContour(true); }

			auto vertices = outline[outline_index].getResampledByCount(100).getVertices();
			for (auto& vertex : vertices) {

				auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 0);
				ofVertex(location);
			}
		}
		ofEndShape(true);

		ofPopMatrix();
	}

	ofSetColor(0);
	this->font.drawString(this->word, this->font.stringWidth(this->word) * -0.5, this->font.stringHeight(this->word) * 0.25);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}