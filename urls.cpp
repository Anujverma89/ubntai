#include "urls.h"

urls::urls() {}

const QString urls::checkemailUrl = "http://localhost:8000/user/finduser";
const QString urls::getOtpUrl = "http://localhost:8000/user/getotp";
const QString urls::verifyOTPurl = "http://localhost:8000/user/verifyotp";
const QString urls::createUserUrl = "http://localhost:8000/user/createuser";
const QString urls::addSystemUrl = "http://localhost:8000/user/addsystem";
const QString urls::saveQuery = "http://localhost:8000/ubntai/savepastqueries";
const QString urls::getPastQuery="http://localhost:8000/ubntai/getpastqueries";
const QString urls::deletePastQuery="http://localhost:8000/ubntai/deletepastqueries";
const QString urls::troubleShoot = "http://localhost:8000/ubntai/troubleshoot";
const QString urls::askQuestion = "http://localhost:8000/ubntai/getanswer";
const QString urls::adderrorreport = "http://localhost:8000/ubntai/adderrorreport";
const QString urls::addapplicationinfo = "http://localhost:8000/ubntai/addapplicationinfo";
