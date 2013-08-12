/*
 * mlds4820.h
 *
 *  Created on: 2012-7-20
 *      Author: Administrator
 */

#ifndef MLDS4820_H_
#define MLDS4820_H_


/***************************** Include Files *********************************/

/************************** Constant Definitions *****************************/

/** @name Instruction List
 *
 * The following list all the CAN instruction in MLDS controller
 *
 * @{
 */
// Set/Get Driver Info
#define MLDS_GID	0x01  /**< Get ID */
#define MLDS_GVER	0x02  /**< Get Version */
#define MLDS_GDTY	0x03  /**< Get type */
#define MLDS_SADR	0x04  /**< Set ID address */
#define MLDS_GADR	0x05  /**< Get ID address */
#define MLDS_GNOD	0x06  /**< Get recent nod info */
#define MLDS_SCBD	0x07  /**< Set CAN speed */
#define MLDS_GCBD	0x08  /**< Get CAN speed */
#define MLDS_GRBD	0x09  /**< Get RS232 speed */
#define MLDS_BAUD	0x0A  /**< Set RS232 speed */
#define MLDS_SNOT	0x0B  /**< Set nod type */
#define MLDS_GNOT	0x0C  /**< Get nod type */

// Configuration Encoder
#define MLDS_KN		0x10  /**< Set Speed constant */
#define MLDS_GKN	0x11  /**< Get Speed constant */
#define MLDS_R		0x12  /**< Set Motor Internal resistance */
#define MLDS_GR		0x13  /**< Get Motor Internal resistance */
#define MLDS_N		0x14  /**< Set Drive ratio */
#define MLDS_GN		0x15  /**< Get Drive ratio */
#define MLDS_SSP	0x16  /**< Set Max speed */
#define MLDS_GSP	0x17  /**< Get Max speed */
#define MLDS_SMV	0x18  /**< Set Min speed */
#define MLDS_GMV	0x19  /**< Get Min speed */
#define MLDS_SPC	0x1A  /**< Set Max Peak Current */
#define MLDS_GPC	0x1B  /**< Get Max Peak Current */
#define MLDS_SCC	0x1C  /**< Set Max Continuous Current */
#define MLDS_GCC	0x1D  /**< Get Max Continuous Current */
#define MLDS_ENC	0x1E  /**< Set Encoder Resolution x4 */
#define MLDS_GENC	0x1F  /**< Get Encoder Resolution x4 */

// Driver Operating mode
#define MLDS_SMOD	0x2A  /**< Set Operating Mode and Signal Source */
#define MLDS_GMOD	0x2B  /**< Get Operating Mode and Signal Source */

// Parameter settings of each Operating mode
#define MLDS_SMAV	0x31  /**< Set Dead zone voltage */
#define MLDS_GMAV	0x32  /**< Get Dead zone voltage */
#define MLDS_SPH	0x33  /**< Set Position Limit Upper */
#define MLDS_GPH	0x34  /**< Get Position Limit Upper */
#define MLDS_SPL	0x35  /**< Set Position Limit Lower */
#define MLDS_GPL	0x36  /**< Get Position Limit Lower */
#define MLDS_STW	0x37  /**< Set Step Width */
#define MLDS_GTW	0x38  /**< Get Step Width */
#define MLDS_SER	0x39  /**< Set Max Position Error */
#define MLDS_GER	0x3A  /**< Get Max Position Error */
#define MLDS_SSK	0x3B  /**< Set Max Step Pulse Frequency */
#define MLDS_GSK	0x3C  /**< Get Max Step Pulse Frequency */
#define MLDS_SPT	0x3D  /**< Set Fault Delay (ms) */
#define MLDS_GPT	0x3E  /**< Get Fault Delay (ms) */
#define MLDS_SPE	0x3F  /**< Set Software Position Limit Enable */
#define MLDS_GPE	0x40  /**< Get Software Position Limit Status */
#define MLDS_SPHE	0x41  /**< Set Hardware Position Limit Enable */
#define MLDS_GPHE	0x42  /**< Get Hardware Position Limit Status */
#define MLDS_GLR	0x43  /**< Not used */
#define MLDS_SL		0x44  /**< Not used */
#define MLDS_SR		0x45  /**< Not used */

// PID Parameter settings
#define MLDS_A		0x58  /**< Set Acceleration */
#define MLDS_GA		0x59  /**< Get Acceleration */
#define MLDS_AP		0x5A  /**< Set Current loop proportional factor */
#define MLDS_GAP	0x5B  /**< Get Current loop proportional factor */
#define MLDS_AI		0x5C  /**< Set Current loop integral factor */
#define MLDS_GAI	0x5D  /**< Get Current loop integral factor */
#define MLDS_AD		0x5E  /**< Set Current loop differential factor */
#define MLDS_GAD	0x5F  /**< Get Current loop differential factor */
#define MLDS_P		0x60  /**< Set Speed loop proportional factor */
#define MLDS_GP		0x61  /**< Get Speed loop proportional factor */
#define MLDS_I		0x62  /**< Set Speed loop integral factor */
#define MLDS_GI		0x63  /**< Get Speed loop integral factor */
#define MLDS_D		0x64  /**< Set Speed loop differential factor */
#define MLDS_GD		0x65  /**< Get Speed loop differential factor */
#define MLDS_MP		0x66  /**< Set Position loop proportional factor */
#define MLDS_GMP	0x67  /**< Get Position loop proportional factor */
#define MLDS_MK		0x68  /**< Set Position loop feedforward factor */
#define MLDS_GMK	0x69  /**< Get Position loop feedforward factor */
#define MLDS_MI		0x6A  /**< Set Position loop integral factor */
#define MLDS_GMI	0x6B  /**< Get Position loop integral factor  */
#define MLDS_MD		0x6C  /**< Set Position loop differential factor */
#define MLDS_GMD	0x6D  /**< Get Position loop differential factor */

// Driver System Control
#define MLDS_ENA	0x80  /**< Load motor */
#define MLDS_DIS	0x81  /**< Unload motor */
#define MLDS_ESA	0x82  /**< Save Parameters to EEPROM */
#define MLDS_FCFG	0x83  /**< Restore to default */
#define MLDS_SBS	0x84  /**< Emergency break */
#define MLDS_CBS	0x85  /**< Return from Emergency Status */

// Driver feedback style
#define MLDS_SSFT	0x8E  /**< Set feedback style */
#define MLDS_GSFT	0x8F  /**< Get feedback style */

// Speed Control Instruction
#define MLDS_V		0x90  /**< Set Motor Speed */
#define MLDS_GV		0x91  /**< Get Motor Average Speed */
#define MLDS_GVE	0x92  /**< Get Motor Speed Error  */
#define MLDS_GSV	0x93  /**< Get Motor Speed set by user */

// Other Control Mode
#define MLDS_AM		0x95  /**< Set PWM percentage */
#define MLDS_EC		0x96  /**< Set Motor Current */

// Position Control Instruction
#define MLDS_PO		0x98  /**< Set Recent as Absolute position */
#define MLDS_M		0x99  /**< Move to position */
#define MLDS_MR		0x9A  /**< Move to position related to last instruction */
#define MLDS_GM		0x9B  /**< Get Recent position */
#define MLDS_GME	0x9C  /**< Get Recent position Error */
#define MLDS_GSM	0x9D  /**< Get position set by user */
#define MLDS_SME	0x9E  /**< Set position Error Range */
#define MLDS_GSE	0x9F  /**< Get position Error Range */

// Limitation Parameters
#define MLDS_SMH	0xA0  /**< Set Positive Position Limits */
#define MLDS_GMH	0xA1  /**< Get Positive Position Limits */
#define MLDS_SML	0xA2  /**< Set Negative Position Limits */
#define MLDS_GML	0xA3  /**< Get Negative Position Limits */
#define MLDS_SVH	0xA4  /**< Set Positive Speed Limits */
#define MLDS_GVH	0xA5  /**< Get Positive Speed Limits */
#define MLDS_SVL	0xA6  /**< Set Negative Speed Limits */
#define MLDS_GVL	0xA7  /**< Get Negative Speed Limits */
#define MLDS_SEH	0xA8  /**< Set Positive Current Limits */
#define MLDS_GEH	0xA9  /**< Get Positive Current Limits */
#define MLDS_SEL	0xAA  /**< Set Negative Current Limits */
#define MLDS_GEL	0xAB  /**< Get Negative Current Limits */

// System Info
#define MLDS_GC		0xD0  /**< Get Current Value  */
#define MLDS_GT		0xD1  /**< Get Temperature */
#define MLDS_GEI	0xD2  /**< Get Error Info */
#define MLDS_GSI	0xD3  /**< Get System Info */

// Driver Ack
#define MLDS_ACK	0x80  /**< Set feedback style */
/*@}*/



#endif /* MLDS4820_H_ */
