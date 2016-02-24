Imports TeensyComm.Parsing

''' <summary>
''' Teensy message class (yes, T stands for Teensy)
''' </summary>
Public Class TMessage

    Private _origID As Int32
    Private _destID As Int32
    Private _cmdNum As SByte
    Private _messageSize As Int32
    Private _cmdData As New List(Of Int32)
    Private _datetime As Date

    Sub New()

    End Sub

    Sub New(Other As TMessage)
        Me._origID = Other._origID
        Me._destID = Other._destID
        Me._cmdNum = Other._cmdNum
        Me._messageSize = Other._messageSize
        Me._cmdData.AddRange(Other._cmdData)
        Me._datetime = Other._datetime
    End Sub

    Public Function Duplicate() As TMessage
        Return New TMessage(Me)
    End Function

    ''' <summary>
    ''' Unique identifier of the message origin (Teensy node, Python listener, Grasshopper listener)
    ''' </summary>
    ''' <returns></returns>
    Public Property OriginID As Int32
        Get
            Return _origID
        End Get
        Set(value As Int32)
            _origID = value
        End Set
    End Property

    ''' <summary>
    ''' Unique identifier of the message destination (Teensy node, Python listener, Grasshopper listener)
    ''' </summary>
    ''' <returns></returns>
    Public Property DestinationID As Int32
        Get
            Return _destID
        End Get
        Set(value As Int32)
            _destID = value
        End Set
    End Property

    ''' <summary>
    ''' Same as cmdNum in the protocol documentation
    ''' </summary>
    ''' <returns></returns>
    Public Property MessageType As MessageType
        Get
            Return _cmdNum
        End Get
        Set(value As MessageType)
            _cmdNum = value
        End Set
    End Property

    ''' <summary>
    ''' For messages which are not in the MessageType enumeration
    ''' </summary>
    ''' <returns></returns>
    Public Property OtherMessageType As SByte
        Get
            Return _cmdNum
        End Get
        Set(value As SByte)
            _cmdNum = value
        End Set
    End Property

    ''' <summary>
    ''' This property is only for internal .NET use - doesn't have any influence on the way the message is parsed to bytes.
    ''' </summary>
    ''' <returns></returns>
    Public Property Time As Date
        Get
            Return _datetime
        End Get
        Set(value As Date)
            _datetime = value
        End Set
    End Property

    ''' <summary>
    ''' 
    ''' </summary>
    ''' <returns></returns>
    Public Property Size As Int32
        Get
            Return _messageSize
        End Get
        Set(value As Int32)
            _messageSize = value
        End Set
    End Property

    ''' <summary>
    ''' When listening, values are always converted to a list of integers (cause integers can store bytes, int16, sbyte etc.)
    ''' </summary>
    ''' <returns></returns>
    Public Property Data As List(Of Int32)
        Get
            Return _cmdData
        End Get
        Set(value As List(Of Int32))
            _cmdData = value
        End Set
    End Property

    Public Overrides Function ToString() As String
        Return _datetime.ToLongTimeString & " Message " & Me.MessageType.ToString & " {Source:" & _origID & " Destination:" & _destID & " ValueCount: " & _cmdData.Count & "}"
    End Function

    Public Overridable Function ToBytes() As Byte()
        Return MsgToBytes(Me)
    End Function

    Public Function IsValid() As Boolean
        Return True
    End Function

End Class

