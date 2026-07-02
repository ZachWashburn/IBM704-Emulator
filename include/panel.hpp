//============================ 2023 @ Zachary Washburn ============================//
// author  : Zachary Washburn
// email   : zachary.washburn.business@gmail.com
// tel     : +1 (571)-354-0641 
// project : IBM704
// dev env : CLion
// DATE    : 11/25/23 10:03 PM
// include/panel.hpp
// 
// Purpose : 
//
//
// Considerations:
//
//====================================================================================//




#ifndef IBM704_PANEL_HPP
#define IBM704_PANEL_HPP

#define LIGHTOFF (false)
#define LIGHTON  (true)

class clight {
public:
    void set(bool _state){state = _state;}
    bool on() const{return state;}
    bool off() const{return !state;}
private:
    bool state = false;
};

// The internal register display shows ip, ic, sr, ac, and mq
// not added here

class cpanellights704 {
public:
    clight& trapindicator(){return m_trapindicator;}
    clight& senselight(int idx){return m_senselights[idx];}
    clight& programstop(){return m_programstop;}
    clight& acoverflow(){return m_acoverflow;}
    clight& mqoverflow(){return m_mqoverflow;}
    clight& dividecheck(){return m_dividecheck;}
    clight& readwriteselect(){return m_readwriteselect;}
    clight& readwritecheck(){return m_readwritecheck;}
    clight& tapecheck(){return m_tapecheck;}
    clight& ready(){return m_readylight;}
    clight& power(){return m_powerlight;}
    clight& automaticlight(){return m_automaticlight;}
protected:
    clight m_trapindicator;
    clight m_senselights[4];
    clight m_programstop;
    clight m_acoverflow;
    clight m_mqoverflow;
    clight m_dividecheck;
    clight m_readwriteselect;
    clight m_readwritecheck;
    clight m_tapecheck;
    clight m_readylight;
    clight m_powerlight;
    clight m_automaticlight;
private:
};

// TODO : Panel Switches and Keys

class cpanel704{
public:
    cpanellights704& lights(){return m_lights;}
protected:
    cpanellights704 m_lights;
private:
};


#endif //IBM704_PANEL_HPP
