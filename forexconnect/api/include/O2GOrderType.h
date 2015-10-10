#pragma once
namespace O2G2
{
namespace Commands
{
    static const char CreateOrder[] = "CreateOrder";
    static const char CreateOCO[] = "CreateOCO";
    static const char CreateOTO[] = "CreateOTO";
    static const char JoinToNewContingencyGroup[] = "JoinToNewContingencyGroup";
    static const char JoinToExistingContingencyGroup[] = "JoinToExistingContingencyGroup";
    static const char RemoveFromContingencyGroup[] = "RemoveFromContingencyGroup";
    static const char EditOrder[] = "EditOrder";
    static const char DeleteOrder[] = "DeleteOrder";
    static const char SetSubscriptionStatus[] = "SetSubscriptionStatus";
    static const char UpdateMarginRequirements[] = "UpdateMarginRequirements";
    static const char GetLastOrderUpdate[] = "GetLastOrderUpdate";
    static const char AcceptOrder[] = "AcceptOrder";
    static const char ChangePassword[] = "ChangePassword";
}
    static const char Buy[] = "B";
    static const char Sell[] = "S";

namespace Orders
{
    static const char TrueMarketOpen[] = "OM";
    static const char MarketOpen[] = "O";
    static const char MarketOpenRange[] = "OR";
    static const char OpenLimit[] = "OL";
    static const char TrueMarketClose[] = "CM";
    static const char MarketClose[] = "C";
    static const char MarketCloseRange[] = "CR";
    static const char CloseLimit[] = "CL";
    static const char StopEntry[] = "SE";
    static const char LimitEntry[] = "LE";
    static const char Entry[] = "E";
    static const char Stop[] = "S";
    static const char Limit[] = "L";
    static const char StopTrailingEntry[] = "STE";
    static const char LimitTrailingEntry[] = "LTE";
}

namespace TIF
{
    static const char GTC[] = "GTC";
    static const char IOC[] = "IOC";
    static const char DAY[] = "DAY";
    static const char FOK[] = "FOK";
    static const char GTD[] = "GTD";
}

namespace Peg
{
    static const char FromOpen[] = "O";
    static const char FromClose[] = "M";
}

namespace KeyType
{
    static const char OrderID[] = "OrderID";
    static const char RequestID[] = "OrderQID";
    static const char RequestTXT[] = "OrderQTXT";
}

namespace SubscriptionStatuses
{
    static const char Tradable[] = "T";
    static const char Disable[] = "D";
    static const char ViewOnly[] = "V";
}

namespace SystemProperties
{
    static const char BASE_CRNCY[] = "BASE_CRNCY";
    static const char BASE_UNIT_SIZE[] = "BASE_UNIT_SIZE";
    static const char BASE_CRNCY_PRECISION[] = "BASE_CRNCY_PRECISION";
    static const char BASE_CRNCY_SYMBOL[] = "BASE_CRNCY_SYMBOL";
    static const char BASE_TIME_ZONE[] = "BASE_TIME_ZONE";
    static const char COND_DIST[] = "COND_DIST";
    static const char COND_DIST_ENTRY[] = "COND_DIST_ENTRY";
    static const char END_TRADING_DAY[] = "END_TRADING_DAY";
    static const char FORCE_PASSWORD_CHANGE[] = "FORCE_PASSWORD_CHANGE";
    static const char MARKET_OPEN[] = "MARKET_OPEN";
    static const char QUERYDEPTH_0[] = "QUERYDEPTH_0";
    static const char QUERYDEPTH_1[] = "QUERYDEPTH_1";
    static const char QUERYDEPTH_2[] = "QUERYDEPTH_2";
    static const char QUERYDEPTH_3[] = "QUERYDEPTH_3";
    static const char QUERYDEPTH_4[] = "QUERYDEPTH_4";
    static const char QUERYDEPTH_5[] = "QUERYDEPTH_5";
    static const char QUERYDEPTH_6[] = "QUERYDEPTH_6";
    static const char QUERYDEPTH_7[] = "QUERYDEPTH_7";
    static const char QUERYDEPTH_8[] = "QUERYDEPTH_8";
    static const char QUERYDEPTH_h2[] = "QUERYDEPTH_h2";
    static const char QUERYDEPTH_h3[] = "QUERYDEPTH_h3";
    static const char QUERYDEPTH_h4[] = "QUERYDEPTH_h4";
    static const char QUERYDEPTH_h6[] = "QUERYDEPTH_h6";
    static const char QUERYDEPTH_h8[] = "QUERYDEPTH_h8";
    static const char SERVER_TIME_UTC[] = "SERVER_TIME_UTC";
    static const char SupportTickVolume[] = "SupportTickVolume";
    static const char TP_170[] = "TP_170";
    static const char TP_171[] = "TP_171";
    static const char TP_172[] = "TP_172";
    static const char TP_86[] = "TP_86";
    static const char TP_88[] = "TP_88";
    static const char TP_89[] = "TP_89";
    static const char TP_94[] = "TP_94";
    static const char CP_170[] = "CP_170";
    static const char CP_171[] = "CP_171";
    static const char CP_172[] = "CP_172";
    static const char CP_86[] = "CP_86";
    static const char CP_88[] = "CP_88";
    static const char CP_89[] = "CP_89";
    static const char CP_94[] = "CP_94";
    static const char TRAILING_DYNAMIC[] = "TRAILING_DYNAMIC";
    static const char TRAILING_FLUCTUATE[] = "TRAILING_FLUCTUATE";
    static const char TRAILING_FLUCTUATE_PTS_MAX[] = "TRAILING_FLUCTUATE_PTS_MAX";
    static const char TRAILING_FLUCTUATE_PTS_MIN[] = "TRAILING_FLUCTUATE_PTS_MIN";
    static const char PEGGED_STOP_LIMIT_DISABLED[] = "PEGGED_STOP_LIMIT_DISABLED";

}
}

