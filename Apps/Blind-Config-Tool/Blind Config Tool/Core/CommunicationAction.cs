using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Blind_Config_Tool.RedesignFiles;

namespace Blind_Config_Tool.Core
{
    public struct CommunicationAction
    {
        public delegate MessageStructure MessageAction(MessageStructure msg);

        MessageAction _preprocessAction;           //The function that will be performed during preprocessing
        readonly ElementActions _elementName;            //Used to easily identify the current script element
        readonly Target _targetAddressType;  //Defines how the program should build the targeting part of the message before sending
        MessageStructure _message;                //Defines the message that will be sent (not including checksums)
        readonly string _description;            //A description of the message
        byte[] _responseType;                      //The starting byte of the message that will come back from the target
        byte[] _responseLength;                            //The length of the message that will come back from the target
        bool _requestsAcknowledgement;           //If true, the function requests an acknowledgement from the slave (Required when calculating the message length as messages that need acknowledgement calculate their length differently)
        Action<MessageStructure> _responseAction;          //The function that will be performed on the response to this message

        #region Constructors
        public CommunicationAction(ElementActions elementActions, Target target, MessageStructure message, string description)
        {
            _targetAddressType = target;
            _message = message;
            _description = description;

            _responseType = new byte[0];
            _responseLength = new byte[0];
            _requestsAcknowledgement = false;
            _responseAction = null;
            _preprocessAction = null;
            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageStructure message, string description, bool requestsAck)
        {
            _targetAddressType = target;
            _message = message;
            _description = description;

            _responseType = new byte[0];
            _responseLength = new byte[0];
            _requestsAcknowledgement = requestsAck;
            _responseAction = null;
            _preprocessAction = null;
            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageAction preprocessAction, MessageStructure message, string description)
        {
            _preprocessAction = preprocessAction;
            _targetAddressType = target;
            _message = message;
            _description = description;

            _responseType = new byte[0];
            _responseLength = new byte[0];
            _requestsAcknowledgement = false;
            _responseAction = null;
            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageAction preprocessAction, MessageStructure message, string description, bool requestsAck)
        {
            _preprocessAction = preprocessAction;
            _targetAddressType = target;
            _message = message;
            _description = description;

            _responseType = new byte[0];
            _responseLength = new byte[0];
            _requestsAcknowledgement = requestsAck;
            _responseAction = null;
            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageStructure message, string description, byte[] responseType, byte[] responseLength, Action<MessageStructure> response)
        {
            _targetAddressType = target;
            _message = message;
            _description = description;
            _responseType = responseType;
            _responseLength = responseLength;
            _requestsAcknowledgement = false;
            _responseAction = response;

            _preprocessAction = null;
            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageStructure message, string description, byte[] responseType, byte[] responseLength, Action<MessageStructure> response, bool requestsAck)
        {
            _targetAddressType = target;
            _message = message;
            _description = description;
            _responseType = responseType;
            _responseLength = responseLength;
            _requestsAcknowledgement = requestsAck;
            _responseAction = response;

            _preprocessAction = null;
            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageAction preprocessAction, MessageStructure message, string description, byte[] responseType, byte[] responseLength, Action<MessageStructure> response)
        {
            _preprocessAction = preprocessAction;
            _targetAddressType = target;
            _message = message;
            _description = description;
            _responseType = responseType;
            _responseLength = responseLength;
            _requestsAcknowledgement = false;
            _responseAction = response;

            _elementName = elementActions;
        }

        public CommunicationAction(ElementActions elementActions, Target target, MessageAction preprocessAction, MessageStructure message, string description, byte[] responseType, byte[] responseLength, Action<MessageStructure> response, bool requestsAck)
        {
            _preprocessAction = preprocessAction;
            _targetAddressType = target;
            _message = message;
            _description = description;
            _responseType = responseType;
            _responseLength = responseLength;
            _requestsAcknowledgement = requestsAck;
            _responseAction = response;

            _elementName = elementActions;
        }
        #endregion

        #region Accessors
        public MessageAction PreprocessAction { get { return _preprocessAction; } }
        public ElementActions ElementName { get { return _elementName; } }
        public Target TargetType { get { return _targetAddressType; } }
        public MessageStructure Message { get { return _message; } }
        public string Description { get { return _description; } }
        public byte[] ResponseType { get { return _responseType; } }
        public byte[] Length { get { return _responseLength; } }
        public Action<MessageStructure> ResponseAction { get { return _responseAction; } }
        public bool RequiresAcknowledgement { get { return _requestsAcknowledgement; } }
        #endregion
    }
}
