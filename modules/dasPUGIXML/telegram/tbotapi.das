options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module tbotapi public

require daslib/json_boost

struct update
    //!  This object represents an incoming update. At most one of the optional parameters can be present in any given update.
    update_id : int	//! The update's unique identifier. Update identifiers start from a certain positive number and increase sequentially. This ID becomes especially handy if you're using webhooks, since it allows you to ignore repeated updates or to restore the correct update sequence, should they get out of order. If there are no new updates for at least a week, then identifier of the next update will be chosen randomly instead of sequentially.
    [[optional]] message : message?	//! Optional. New incoming message of any kind - text, photo, sticker, etc.
    [[optional]] edited_message : message?	//! Optional. New version of a message that is known to the bot and was edited
    [[optional]] channel_post : message?	//! Optional. New incoming channel post of any kind - text, photo, sticker, etc.
    [[optional]] edited_channel_post : message?	//! Optional. New version of a channel post that is known to the bot and was edited
    [[optional]] inline_query : inlinequery?	//! Optional. New incoming inline query
    [[optional]] chosen_inline_result : choseninlineresult?	//! Optional. The result of an inline query that was chosen by a user and sent to their chat partner. Please see our documentation on the feedback collecting for details on how to enable these updates for your bot.
    [[optional]] callback_query : callbackquery?	//! Optional. New incoming callback query
    [[optional]] shipping_query : shippingquery?	//! Optional. New incoming shipping query. Only for invoices with flexible price
    [[optional]] pre_checkout_query : precheckoutquery?	//! Optional. New incoming pre-checkout query. Contains full information about checkout
    [[optional]] poll : poll?	//! Optional. New poll state. Bots receive only updates about stopped polls and polls, which are sent by the bot
    [[optional]] poll_answer : pollanswer?	//! Optional. A user changed their answer in a non-anonymous poll. Bots receive new votes only in polls that were sent by the bot itself.
    [[optional]] my_chat_member : chatmemberupdated?	//! Optional. The bot's chat member status was updated in a chat. For private chats, this update is received only when the bot is blocked or unblocked by the user.
    [[optional]] chat_member : chatmemberupdated?	//! Optional. A chat member's status was updated in a chat. The bot must be an administrator in the chat and must explicitly specify “chat_member” in the list of allowed_updates to receive these updates.
    [[optional]] chat_join_request : chatjoinrequest?	//! Optional. A request to join the chat has been sent. The bot must have the can_invite_users administrator right in the chat to receive these updates.

def from_JV ( var v:JsonValue?; t:update ) : update
    var q : update
    move_to_ref(q.update_id, _::from_JV((v as _object)["update_id"], type<int>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<message?>))
    move_to_ref(q.edited_message, _::from_JV((v as _object)["edited_message"], type<message?>))
    move_to_ref(q.channel_post, _::from_JV((v as _object)["channel_post"], type<message?>))
    move_to_ref(q.edited_channel_post, _::from_JV((v as _object)["edited_channel_post"], type<message?>))
    move_to_ref(q.inline_query, _::from_JV((v as _object)["inline_query"], type<inlinequery?>))
    move_to_ref(q.chosen_inline_result, _::from_JV((v as _object)["chosen_inline_result"], type<choseninlineresult?>))
    move_to_ref(q.callback_query, _::from_JV((v as _object)["callback_query"], type<callbackquery?>))
    move_to_ref(q.shipping_query, _::from_JV((v as _object)["shipping_query"], type<shippingquery?>))
    move_to_ref(q.pre_checkout_query, _::from_JV((v as _object)["pre_checkout_query"], type<precheckoutquery?>))
    move_to_ref(q.poll, _::from_JV((v as _object)["poll"], type<poll?>))
    move_to_ref(q.poll_answer, _::from_JV((v as _object)["poll_answer"], type<pollanswer?>))
    move_to_ref(q.my_chat_member, _::from_JV((v as _object)["my_chat_member"], type<chatmemberupdated?>))
    move_to_ref(q.chat_member, _::from_JV((v as _object)["chat_member"], type<chatmemberupdated?>))
    move_to_ref(q.chat_join_request, _::from_JV((v as _object)["chat_join_request"], type<chatjoinrequest?>))
    return <- q

def from_JV ( var v:JsonValue?; t:update? ) : update?
    return null if v==null
    var q = new update
    *q <- from_JV(v,type<update>)
    return q

struct getupdates
    //!  Use this method to receive incoming updates using long polling (wiki). Returns an Array of Update objects.
    [[optional]] offset : int	//! Optional. Identifier of the first update to be returned. Must be greater by one than the highest among the identifiers of previously received updates. By default, updates starting with the earliest unconfirmed update are returned. An update is considered confirmed as soon as getUpdates is called with an offset higher than its update_id. The negative offset can be specified to retrieve updates starting from -offset update from the end of the updates queue. All previous updates will forgotten.
    [[optional]] limit : int	//! Optional. Limits the number of updates to be retrieved. Values between 1-100 are accepted. Defaults to 100.
    [[optional]] timeout : int	//! Optional. Timeout in seconds for long polling. Defaults to 0, i.e. usual short polling. Should be positive, short polling should be used for testing purposes only.
    [[optional]] allowed_updates : array<string>	//! Optional. A JSON-serialized list of the update types you want your bot to receive. For example, specify [“message”, “edited_channel_post”, “callback_query”] to only receive updates of these types. See Update for a complete list of available update types. Specify an empty list to receive all update types except chat_member (default). If not specified, the previous setting will be used. Please note that this parameter doesn't affect updates created before the call to the getUpdates, so unwanted updates may be received for a short period of time.

def from_JV ( var v:JsonValue?; t:getupdates ) : getupdates
    var q : getupdates
    move_to_ref(q.offset, _::from_JV((v as _object)["offset"], type<int>))
    move_to_ref(q.limit, _::from_JV((v as _object)["limit"], type<int>))
    move_to_ref(q.timeout, _::from_JV((v as _object)["timeout"], type<int>))
    move_to_ref(q.allowed_updates, _::from_JV((v as _object)["allowed_updates"], type<array<string>>))
    return <- q

def from_JV ( var v:JsonValue?; t:getupdates? ) : getupdates?
    return null if v==null
    var q = new getupdates
    *q <- from_JV(v,type<getupdates>)
    return q

struct setwebhook
    //!  Use this method to specify a URL and receive incoming updates via an outgoing webhook. Whenever there is an update for the bot, we will send an HTTPS POST request to the specified URL, containing a JSON-serialized Update. In case of an unsuccessful request, we will give up after a reasonable amount of attempts. Returns True on success. If you'd like to make sure that the webhook was set by you, you can specify secret data in the parameter secret_token. If specified, the request will contain a header “X-Telegram-Bot-Api-Secret-Token” with the secret token as content.
    url : string	//! Required. HTTPS URL to send updates to. Use an empty string to remove webhook integration
    [[optional]] certificate : inputfile?	//! Optional. Upload your public key certificate so that the root certificate in use can be checked. See our self-signed guide for details.
    [[optional]] ip_address : string	//! Optional. The fixed IP address which will be used to send webhook requests instead of the IP address resolved through DNS
    [[optional]] max_connections : int	//! Optional. The maximum allowed number of simultaneous HTTPS connections to the webhook for update delivery, 1-100. Defaults to 40. Use lower values to limit the load on your bot's server, and higher values to increase your bot's throughput.
    [[optional]] allowed_updates : array<string>	//! Optional. A JSON-serialized list of the update types you want your bot to receive. For example, specify [“message”, “edited_channel_post”, “callback_query”] to only receive updates of these types. See Update for a complete list of available update types. Specify an empty list to receive all update types except chat_member (default). If not specified, the previous setting will be used. Please note that this parameter doesn't affect updates created before the call to the setWebhook, so unwanted updates may be received for a short period of time.
    [[optional]] drop_pending_updates : bool	//! Optional. Pass True to drop all pending updates
    [[optional]] secret_token : string	//! Optional. A secret token to be sent in a header “X-Telegram-Bot-Api-Secret-Token” in every webhook request, 1-256 characters. Only characters A-Z, a-z, 0-9, _ and - are allowed. The header is useful to ensure that the request comes from a webhook set by you.

def from_JV ( var v:JsonValue?; t:setwebhook ) : setwebhook
    var q : setwebhook
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.certificate, _::from_JV((v as _object)["certificate"], type<inputfile?>))
    move_to_ref(q.ip_address, _::from_JV((v as _object)["ip_address"], type<string>))
    move_to_ref(q.max_connections, _::from_JV((v as _object)["max_connections"], type<int>))
    move_to_ref(q.allowed_updates, _::from_JV((v as _object)["allowed_updates"], type<array<string>>))
    move_to_ref(q.drop_pending_updates, _::from_JV((v as _object)["drop_pending_updates"], type<bool>))
    move_to_ref(q.secret_token, _::from_JV((v as _object)["secret_token"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setwebhook? ) : setwebhook?
    return null if v==null
    var q = new setwebhook
    *q <- from_JV(v,type<setwebhook>)
    return q

struct deletewebhook
    //!  Use this method to remove webhook integration if you decide to switch back to getUpdates. Returns True on success.
    [[optional]] drop_pending_updates : bool	//! Optional. Pass True to drop all pending updates

def from_JV ( var v:JsonValue?; t:deletewebhook ) : deletewebhook
    var q : deletewebhook
    move_to_ref(q.drop_pending_updates, _::from_JV((v as _object)["drop_pending_updates"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletewebhook? ) : deletewebhook?
    return null if v==null
    var q = new deletewebhook
    *q <- from_JV(v,type<deletewebhook>)
    return q

struct getwebhookinfo
    //!  Use this method to get current webhook status. Requires no parameters. On success, returns a WebhookInfo object. If the bot is using getUpdates, will return an object with the url field empty.
    object_getwebhookinfo_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:getwebhookinfo ) : getwebhookinfo
    var q : getwebhookinfo
    return <- q

def from_JV ( var v:JsonValue?; t:getwebhookinfo? ) : getwebhookinfo?
    return null if v==null
    var q = new getwebhookinfo
    *q <- from_JV(v,type<getwebhookinfo>)
    return q

struct webhookinfo
    //!  Describes the current status of a webhook.
    url : string	//! Webhook URL, may be empty if webhook is not set up
    has_custom_certificate : bool	//! True, if a custom certificate was provided for webhook certificate checks
    pending_update_count : int	//! Number of updates awaiting delivery
    [[optional]] ip_address : string	//! Optional. Currently used webhook IP address
    [[optional]] last_error_date : int	//! Optional. Unix time for the most recent error that happened when trying to deliver an update via webhook
    [[optional]] last_error_message : string	//! Optional. Error message in human-readable format for the most recent error that happened when trying to deliver an update via webhook
    [[optional]] last_synchronization_error_date : int	//! Optional. Unix time of the most recent error that happened when trying to synchronize available updates with Telegram datacenters
    [[optional]] max_connections : int	//! Optional. The maximum allowed number of simultaneous HTTPS connections to the webhook for update delivery
    [[optional]] allowed_updates : array<string>	//! Optional. A list of update types the bot is subscribed to. Defaults to all update types except chat_member

def from_JV ( var v:JsonValue?; t:webhookinfo ) : webhookinfo
    var q : webhookinfo
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.has_custom_certificate, _::from_JV((v as _object)["has_custom_certificate"], type<bool>))
    move_to_ref(q.pending_update_count, _::from_JV((v as _object)["pending_update_count"], type<int>))
    move_to_ref(q.ip_address, _::from_JV((v as _object)["ip_address"], type<string>))
    move_to_ref(q.last_error_date, _::from_JV((v as _object)["last_error_date"], type<int>))
    move_to_ref(q.last_error_message, _::from_JV((v as _object)["last_error_message"], type<string>))
    move_to_ref(q.last_synchronization_error_date, _::from_JV((v as _object)["last_synchronization_error_date"], type<int>))
    move_to_ref(q.max_connections, _::from_JV((v as _object)["max_connections"], type<int>))
    move_to_ref(q.allowed_updates, _::from_JV((v as _object)["allowed_updates"], type<array<string>>))
    return <- q

def from_JV ( var v:JsonValue?; t:webhookinfo? ) : webhookinfo?
    return null if v==null
    var q = new webhookinfo
    *q <- from_JV(v,type<webhookinfo>)
    return q

struct user
    //!  This object represents a Telegram user or bot.
    id : int	//! Unique identifier for this user or bot. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier.
    is_bot : bool	//! True, if this user is a bot
    first_name : string	//! User's or bot's first name
    [[optional]] last_name : string	//! Optional. User's or bot's last name
    [[optional]] username : string	//! Optional. User's or bot's username
    [[optional]] language_code : string	//! Optional. IETF language tag of the user's language
    [[optional]] is_premium : bool	//! Optional. True, if this user is a Telegram Premium user
    [[optional]] added_to_attachment_menu : bool	//! Optional. True, if this user added the bot to the attachment menu
    [[optional]] can_join_groups : bool	//! Optional. True, if the bot can be invited to groups. Returned only in getMe.
    [[optional]] can_read_all_group_messages : bool	//! Optional. True, if privacy mode is disabled for the bot. Returned only in getMe.
    [[optional]] supports_inline_queries : bool	//! Optional. True, if the bot supports inline queries. Returned only in getMe.

def from_JV ( var v:JsonValue?; t:user ) : user
    var q : user
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<int>))
    move_to_ref(q.is_bot, _::from_JV((v as _object)["is_bot"], type<bool>))
    move_to_ref(q.first_name, _::from_JV((v as _object)["first_name"], type<string>))
    move_to_ref(q.last_name, _::from_JV((v as _object)["last_name"], type<string>))
    move_to_ref(q.username, _::from_JV((v as _object)["username"], type<string>))
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    move_to_ref(q.is_premium, _::from_JV((v as _object)["is_premium"], type<bool>))
    move_to_ref(q.added_to_attachment_menu, _::from_JV((v as _object)["added_to_attachment_menu"], type<bool>))
    move_to_ref(q.can_join_groups, _::from_JV((v as _object)["can_join_groups"], type<bool>))
    move_to_ref(q.can_read_all_group_messages, _::from_JV((v as _object)["can_read_all_group_messages"], type<bool>))
    move_to_ref(q.supports_inline_queries, _::from_JV((v as _object)["supports_inline_queries"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:user? ) : user?
    return null if v==null
    var q = new user
    *q <- from_JV(v,type<user>)
    return q

struct chat
    //!  This object represents a chat.
    id : int	//! Unique identifier for this chat. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    _type : string	//! Type of chat, can be either “private”, “group”, “supergroup” or “channel”
    [[optional]] title : string	//! Optional. Title, for supergroups, channels and group chats
    [[optional]] username : string	//! Optional. Username, for private chats, supergroups and channels if available
    [[optional]] first_name : string	//! Optional. First name of the other party in a private chat
    [[optional]] last_name : string	//! Optional. Last name of the other party in a private chat
    [[optional]] is_forum : bool	//! Optional. True, if the supergroup chat is a forum (has topics enabled)
    [[optional]] photo : chatphoto?	//! Optional. Chat photo. Returned only in getChat.
    [[optional]] active_usernames : array<string>	//! Optional. If non-empty, the list of all active chat usernames; for private chats, supergroups and channels. Returned only in getChat.
    [[optional]] emoji_status_custom_emoji_id : string	//! Optional. Custom emoji identifier of emoji status of the other party in a private chat. Returned only in getChat.
    [[optional]] bio : string	//! Optional. Bio of the other party in a private chat. Returned only in getChat.
    [[optional]] has_private_forwards : bool	//! Optional. True, if privacy settings of the other party in the private chat allows to use tg://user?id=<user_id> links only in chats with the user. Returned only in getChat.
    [[optional]] has_restricted_voice_and_video_messages : bool	//! Optional. True, if the privacy settings of the other party restrict sending voice and video note messages in the private chat. Returned only in getChat.
    [[optional]] join_to_send_messages : bool	//! Optional. True, if users need to join the supergroup before they can send messages. Returned only in getChat.
    [[optional]] join_by_request : bool	//! Optional. True, if all users directly joining the supergroup need to be approved by supergroup administrators. Returned only in getChat.
    [[optional]] description : string	//! Optional. Description, for groups, supergroups and channel chats. Returned only in getChat.
    [[optional]] invite_link : string	//! Optional. Primary invite link, for groups, supergroups and channel chats. Returned only in getChat.
    [[optional]] pinned_message : message?	//! Optional. The most recent pinned message (by sending date). Returned only in getChat.
    [[optional]] permissions : chatpermissions?	//! Optional. Default chat member permissions, for groups and supergroups. Returned only in getChat.
    [[optional]] slow_mode_delay : int	//! Optional. For supergroups, the minimum allowed delay between consecutive messages sent by each unpriviledged user; in seconds. Returned only in getChat.
    [[optional]] message_auto_delete_time : int	//! Optional. The time after which all messages sent to the chat will be automatically deleted; in seconds. Returned only in getChat.
    [[optional]] has_aggressive_anti_spam_enabled : bool	//! Optional. True, if aggressive anti-spam checks are enabled in the supergroup. The field is only available to chat administrators. Returned only in getChat.
    [[optional]] has_hidden_members : bool	//! Optional. True, if non-administrators can only get the list of bots and administrators in the chat. Returned only in getChat.
    [[optional]] has_protected_content : bool	//! Optional. True, if messages from the chat can't be forwarded to other chats. Returned only in getChat.
    [[optional]] sticker_set_name : string	//! Optional. For supergroups, name of group sticker set. Returned only in getChat.
    [[optional]] can_set_sticker_set : bool	//! Optional. True, if the bot can change the group sticker set. Returned only in getChat.
    [[optional]] linked_chat_id : int	//! Optional. Unique identifier for the linked chat, i.e. the discussion group identifier for a channel and vice versa; for supergroups and channel chats. This identifier may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier. Returned only in getChat.
    [[optional]] location : chatlocation?	//! Optional. For supergroups, the location to which the supergroup is connected. Returned only in getChat.

def from_JV ( var v:JsonValue?; t:chat ) : chat
    var q : chat
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<int>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.username, _::from_JV((v as _object)["username"], type<string>))
    move_to_ref(q.first_name, _::from_JV((v as _object)["first_name"], type<string>))
    move_to_ref(q.last_name, _::from_JV((v as _object)["last_name"], type<string>))
    move_to_ref(q.is_forum, _::from_JV((v as _object)["is_forum"], type<bool>))
    move_to_ref(q.photo, _::from_JV((v as _object)["photo"], type<chatphoto?>))
    move_to_ref(q.active_usernames, _::from_JV((v as _object)["active_usernames"], type<array<string>>))
    move_to_ref(q.emoji_status_custom_emoji_id, _::from_JV((v as _object)["emoji_status_custom_emoji_id"], type<string>))
    move_to_ref(q.bio, _::from_JV((v as _object)["bio"], type<string>))
    move_to_ref(q.has_private_forwards, _::from_JV((v as _object)["has_private_forwards"], type<bool>))
    move_to_ref(q.has_restricted_voice_and_video_messages, _::from_JV((v as _object)["has_restricted_voice_and_video_messages"], type<bool>))
    move_to_ref(q.join_to_send_messages, _::from_JV((v as _object)["join_to_send_messages"], type<bool>))
    move_to_ref(q.join_by_request, _::from_JV((v as _object)["join_by_request"], type<bool>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.invite_link, _::from_JV((v as _object)["invite_link"], type<string>))
    move_to_ref(q.pinned_message, _::from_JV((v as _object)["pinned_message"], type<message?>))
    move_to_ref(q.permissions, _::from_JV((v as _object)["permissions"], type<chatpermissions?>))
    move_to_ref(q.slow_mode_delay, _::from_JV((v as _object)["slow_mode_delay"], type<int>))
    move_to_ref(q.message_auto_delete_time, _::from_JV((v as _object)["message_auto_delete_time"], type<int>))
    move_to_ref(q.has_aggressive_anti_spam_enabled, _::from_JV((v as _object)["has_aggressive_anti_spam_enabled"], type<bool>))
    move_to_ref(q.has_hidden_members, _::from_JV((v as _object)["has_hidden_members"], type<bool>))
    move_to_ref(q.has_protected_content, _::from_JV((v as _object)["has_protected_content"], type<bool>))
    move_to_ref(q.sticker_set_name, _::from_JV((v as _object)["sticker_set_name"], type<string>))
    move_to_ref(q.can_set_sticker_set, _::from_JV((v as _object)["can_set_sticker_set"], type<bool>))
    move_to_ref(q.linked_chat_id, _::from_JV((v as _object)["linked_chat_id"], type<int>))
    move_to_ref(q.location, _::from_JV((v as _object)["location"], type<chatlocation?>))
    return <- q

def from_JV ( var v:JsonValue?; t:chat? ) : chat?
    return null if v==null
    var q = new chat
    *q <- from_JV(v,type<chat>)
    return q

struct message
    //!  This object represents a message.
    message_id : int	//! Unique message identifier inside this chat
    [[optional]] message_thread_id : int	//! Optional. Unique identifier of a message thread to which the message belongs; for supergroups only
    [[optional]] from : user?	//! Optional. Sender of the message; empty for messages sent to channels. For backward compatibility, the field contains a fake sender user in non-channel chats, if the message was sent on behalf of a chat.
    [[optional]] sender_chat : chat?	//! Optional. Sender of the message, sent on behalf of a chat. For example, the channel itself for channel posts, the supergroup itself for messages from anonymous group administrators, the linked channel for messages automatically forwarded to the discussion group. For backward compatibility, the field from contains a fake sender user in non-channel chats, if the message was sent on behalf of a chat.
    date : int	//! Date the message was sent in Unix time
    chat : chat	//! Conversation the message belongs to
    [[optional]] forward_from : user?	//! Optional. For forwarded messages, sender of the original message
    [[optional]] forward_from_chat : chat?	//! Optional. For messages forwarded from channels or from anonymous administrators, information about the original sender chat
    [[optional]] forward_from_message_id : int	//! Optional. For messages forwarded from channels, identifier of the original message in the channel
    [[optional]] forward_signature : string	//! Optional. For forwarded messages that were originally sent in channels or by an anonymous chat administrator, signature of the message sender if present
    [[optional]] forward_sender_name : string	//! Optional. Sender's name for messages forwarded from users who disallow adding a link to their account in forwarded messages
    [[optional]] forward_date : int	//! Optional. For forwarded messages, date the original message was sent in Unix time
    [[optional]] is_topic_message : bool	//! Optional. True, if the message is sent to a forum topic
    [[optional]] is_automatic_forward : bool	//! Optional. True, if the message is a channel post that was automatically forwarded to the connected discussion group
    [[optional]] reply_to_message : message?	//! Optional. For replies, the original message. Note that the Message object in this field will not contain further reply_to_message fields even if it itself is a reply.
    [[optional]] via_bot : user?	//! Optional. Bot through which the message was sent
    [[optional]] edit_date : int	//! Optional. Date the message was last edited in Unix time
    [[optional]] has_protected_content : bool	//! Optional. True, if the message can't be forwarded
    [[optional]] media_group_id : string	//! Optional. The unique identifier of a media message group this message belongs to
    [[optional]] author_signature : string	//! Optional. Signature of the post author for messages in channels, or the custom title of an anonymous group administrator
    [[optional]] text : string	//! Optional. For text messages, the actual UTF-8 text of the message
    [[optional]] entities : array<messageentity>	//! Optional. For text messages, special entities like usernames, URLs, bot commands, etc. that appear in the text
    [[optional]] animation : animation?	//! Optional. Message is an animation, information about the animation. For backward compatibility, when this field is set, the document field will also be set
    [[optional]] audio : audio?	//! Optional. Message is an audio file, information about the file
    [[optional]] document : document?	//! Optional. Message is a general file, information about the file
    [[optional]] photo : array<photosize>	//! Optional. Message is a photo, available sizes of the photo
    [[optional]] sticker : sticker?	//! Optional. Message is a sticker, information about the sticker
    [[optional]] video : video?	//! Optional. Message is a video, information about the video
    [[optional]] video_note : videonote?	//! Optional. Message is a video note, information about the video message
    [[optional]] voice : voice?	//! Optional. Message is a voice message, information about the file
    [[optional]] caption : string	//! Optional. Caption for the animation, audio, document, photo, video or voice
    [[optional]] caption_entities : array<messageentity>	//! Optional. For messages with a caption, special entities like usernames, URLs, bot commands, etc. that appear in the caption
    [[optional]] has_media_spoiler : bool	//! Optional. True, if the message media is covered by a spoiler animation
    [[optional]] contact : contact?	//! Optional. Message is a shared contact, information about the contact
    [[optional]] dice : dice?	//! Optional. Message is a dice with random value
    [[optional]] game : game?	//! Optional. Message is a game, information about the game. More about games »
    [[optional]] poll : poll?	//! Optional. Message is a native poll, information about the poll
    [[optional]] venue : venue?	//! Optional. Message is a venue, information about the venue. For backward compatibility, when this field is set, the location field will also be set
    [[optional]] location : location?	//! Optional. Message is a shared location, information about the location
    [[optional]] new_chat_members : array<user>	//! Optional. New members that were added to the group or supergroup and information about them (the bot itself may be one of these members)
    [[optional]] left_chat_member : user?	//! Optional. A member was removed from the group, information about them (this member may be the bot itself)
    [[optional]] new_chat_title : string	//! Optional. A chat title was changed to this value
    [[optional]] new_chat_photo : array<photosize>	//! Optional. A chat photo was change to this value
    [[optional]] delete_chat_photo : bool	//! Optional. Service message: the chat photo was deleted
    [[optional]] group_chat_created : bool	//! Optional. Service message: the group has been created
    [[optional]] supergroup_chat_created : bool	//! Optional. Service message: the supergroup has been created. This field can't be received in a message coming through updates, because bot can't be a member of a supergroup when it is created. It can only be found in reply_to_message if someone replies to a very first message in a directly created supergroup.
    [[optional]] channel_chat_created : bool	//! Optional. Service message: the channel has been created. This field can't be received in a message coming through updates, because bot can't be a member of a channel when it is created. It can only be found in reply_to_message if someone replies to a very first message in a channel.
    [[optional]] message_auto_delete_timer_changed : messageautodeletetimerchanged?	//! Optional. Service message: auto-delete timer settings changed in the chat
    [[optional]] migrate_to_chat_id : int	//! Optional. The group has been migrated to a supergroup with the specified identifier. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] migrate_from_chat_id : int	//! Optional. The supergroup has been migrated from a group with the specified identifier. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] pinned_message : message?	//! Optional. Specified message was pinned. Note that the Message object in this field will not contain further reply_to_message fields even if it is itself a reply.
    [[optional]] invoice : invoice?	//! Optional. Message is an invoice for a payment, information about the invoice. More about payments »
    [[optional]] successful_payment : successfulpayment?	//! Optional. Message is a service message about a successful payment, information about the payment. More about payments »
    [[optional]] user_shared : usershared?	//! Optional. Service message: a user was shared with the bot
    [[optional]] chat_shared : chatshared?	//! Optional. Service message: a chat was shared with the bot
    [[optional]] connected_website : string	//! Optional. The domain name of the website on which the user has logged in. More about Telegram Login »
    [[optional]] write_access_allowed : writeaccessallowed?	//! Optional. Service message: the user allowed the bot added to the attachment menu to write messages
    [[optional]] passport_data : passportdata?	//! Optional. Telegram Passport data
    [[optional]] proximity_alert_triggered : proximityalerttriggered?	//! Optional. Service message. A user in the chat triggered another user's proximity alert while sharing Live Location.
    [[optional]] forum_topic_created : forumtopiccreated?	//! Optional. Service message: forum topic created
    [[optional]] forum_topic_edited : forumtopicedited?	//! Optional. Service message: forum topic edited
    [[optional]] forum_topic_closed : forumtopicclosed?	//! Optional. Service message: forum topic closed
    [[optional]] forum_topic_reopened : forumtopicreopened?	//! Optional. Service message: forum topic reopened
    [[optional]] general_forum_topic_hidden : generalforumtopichidden?	//! Optional. Service message: the 'General' forum topic hidden
    [[optional]] general_forum_topic_unhidden : generalforumtopicunhidden?	//! Optional. Service message: the 'General' forum topic unhidden
    [[optional]] video_chat_scheduled : videochatscheduled?	//! Optional. Service message: video chat scheduled
    [[optional]] video_chat_started : videochatstarted?	//! Optional. Service message: video chat started
    [[optional]] video_chat_ended : videochatended?	//! Optional. Service message: video chat ended
    [[optional]] video_chat_participants_invited : videochatparticipantsinvited?	//! Optional. Service message: new participants invited to a video chat
    [[optional]] web_app_data : webappdata?	//! Optional. Service message: data sent by a Web App
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message. login_url buttons are represented as ordinary url buttons.

def from_JV ( var v:JsonValue?; t:message ) : message
    var q : message
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user?>))
    move_to_ref(q.sender_chat, _::from_JV((v as _object)["sender_chat"], type<chat?>))
    move_to_ref(q.date, _::from_JV((v as _object)["date"], type<int>))
    move_to_ref(q.chat, _::from_JV((v as _object)["chat"], type<chat>))
    move_to_ref(q.forward_from, _::from_JV((v as _object)["forward_from"], type<user?>))
    move_to_ref(q.forward_from_chat, _::from_JV((v as _object)["forward_from_chat"], type<chat?>))
    move_to_ref(q.forward_from_message_id, _::from_JV((v as _object)["forward_from_message_id"], type<int>))
    move_to_ref(q.forward_signature, _::from_JV((v as _object)["forward_signature"], type<string>))
    move_to_ref(q.forward_sender_name, _::from_JV((v as _object)["forward_sender_name"], type<string>))
    move_to_ref(q.forward_date, _::from_JV((v as _object)["forward_date"], type<int>))
    move_to_ref(q.is_topic_message, _::from_JV((v as _object)["is_topic_message"], type<bool>))
    move_to_ref(q.is_automatic_forward, _::from_JV((v as _object)["is_automatic_forward"], type<bool>))
    move_to_ref(q.reply_to_message, _::from_JV((v as _object)["reply_to_message"], type<message?>))
    move_to_ref(q.via_bot, _::from_JV((v as _object)["via_bot"], type<user?>))
    move_to_ref(q.edit_date, _::from_JV((v as _object)["edit_date"], type<int>))
    move_to_ref(q.has_protected_content, _::from_JV((v as _object)["has_protected_content"], type<bool>))
    move_to_ref(q.media_group_id, _::from_JV((v as _object)["media_group_id"], type<string>))
    move_to_ref(q.author_signature, _::from_JV((v as _object)["author_signature"], type<string>))
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.entities, _::from_JV((v as _object)["entities"], type<array<messageentity>>))
    move_to_ref(q.animation, _::from_JV((v as _object)["animation"], type<animation?>))
    move_to_ref(q.audio, _::from_JV((v as _object)["audio"], type<audio?>))
    move_to_ref(q.document, _::from_JV((v as _object)["document"], type<document?>))
    move_to_ref(q.photo, _::from_JV((v as _object)["photo"], type<array<photosize>>))
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<sticker?>))
    move_to_ref(q.video, _::from_JV((v as _object)["video"], type<video?>))
    move_to_ref(q.video_note, _::from_JV((v as _object)["video_note"], type<videonote?>))
    move_to_ref(q.voice, _::from_JV((v as _object)["voice"], type<voice?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.has_media_spoiler, _::from_JV((v as _object)["has_media_spoiler"], type<bool>))
    move_to_ref(q.contact, _::from_JV((v as _object)["contact"], type<contact?>))
    move_to_ref(q.dice, _::from_JV((v as _object)["dice"], type<dice?>))
    move_to_ref(q.game, _::from_JV((v as _object)["game"], type<game?>))
    move_to_ref(q.poll, _::from_JV((v as _object)["poll"], type<poll?>))
    move_to_ref(q.venue, _::from_JV((v as _object)["venue"], type<venue?>))
    move_to_ref(q.location, _::from_JV((v as _object)["location"], type<location?>))
    move_to_ref(q.new_chat_members, _::from_JV((v as _object)["new_chat_members"], type<array<user>>))
    move_to_ref(q.left_chat_member, _::from_JV((v as _object)["left_chat_member"], type<user?>))
    move_to_ref(q.new_chat_title, _::from_JV((v as _object)["new_chat_title"], type<string>))
    move_to_ref(q.new_chat_photo, _::from_JV((v as _object)["new_chat_photo"], type<array<photosize>>))
    move_to_ref(q.delete_chat_photo, _::from_JV((v as _object)["delete_chat_photo"], type<bool>))
    move_to_ref(q.group_chat_created, _::from_JV((v as _object)["group_chat_created"], type<bool>))
    move_to_ref(q.supergroup_chat_created, _::from_JV((v as _object)["supergroup_chat_created"], type<bool>))
    move_to_ref(q.channel_chat_created, _::from_JV((v as _object)["channel_chat_created"], type<bool>))
    move_to_ref(q.message_auto_delete_timer_changed, _::from_JV((v as _object)["message_auto_delete_timer_changed"], type<messageautodeletetimerchanged?>))
    move_to_ref(q.migrate_to_chat_id, _::from_JV((v as _object)["migrate_to_chat_id"], type<int>))
    move_to_ref(q.migrate_from_chat_id, _::from_JV((v as _object)["migrate_from_chat_id"], type<int>))
    move_to_ref(q.pinned_message, _::from_JV((v as _object)["pinned_message"], type<message?>))
    move_to_ref(q.invoice, _::from_JV((v as _object)["invoice"], type<invoice?>))
    move_to_ref(q.successful_payment, _::from_JV((v as _object)["successful_payment"], type<successfulpayment?>))
    move_to_ref(q.user_shared, _::from_JV((v as _object)["user_shared"], type<usershared?>))
    move_to_ref(q.chat_shared, _::from_JV((v as _object)["chat_shared"], type<chatshared?>))
    move_to_ref(q.connected_website, _::from_JV((v as _object)["connected_website"], type<string>))
    move_to_ref(q.write_access_allowed, _::from_JV((v as _object)["write_access_allowed"], type<writeaccessallowed?>))
    move_to_ref(q.passport_data, _::from_JV((v as _object)["passport_data"], type<passportdata?>))
    move_to_ref(q.proximity_alert_triggered, _::from_JV((v as _object)["proximity_alert_triggered"], type<proximityalerttriggered?>))
    move_to_ref(q.forum_topic_created, _::from_JV((v as _object)["forum_topic_created"], type<forumtopiccreated?>))
    move_to_ref(q.forum_topic_edited, _::from_JV((v as _object)["forum_topic_edited"], type<forumtopicedited?>))
    move_to_ref(q.forum_topic_closed, _::from_JV((v as _object)["forum_topic_closed"], type<forumtopicclosed?>))
    move_to_ref(q.forum_topic_reopened, _::from_JV((v as _object)["forum_topic_reopened"], type<forumtopicreopened?>))
    move_to_ref(q.general_forum_topic_hidden, _::from_JV((v as _object)["general_forum_topic_hidden"], type<generalforumtopichidden?>))
    move_to_ref(q.general_forum_topic_unhidden, _::from_JV((v as _object)["general_forum_topic_unhidden"], type<generalforumtopicunhidden?>))
    move_to_ref(q.video_chat_scheduled, _::from_JV((v as _object)["video_chat_scheduled"], type<videochatscheduled?>))
    move_to_ref(q.video_chat_started, _::from_JV((v as _object)["video_chat_started"], type<videochatstarted?>))
    move_to_ref(q.video_chat_ended, _::from_JV((v as _object)["video_chat_ended"], type<videochatended?>))
    move_to_ref(q.video_chat_participants_invited, _::from_JV((v as _object)["video_chat_participants_invited"], type<videochatparticipantsinvited?>))
    move_to_ref(q.web_app_data, _::from_JV((v as _object)["web_app_data"], type<webappdata?>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:message? ) : message?
    return null if v==null
    var q = new message
    *q <- from_JV(v,type<message>)
    return q

struct messageid
    //!  This object represents a unique message identifier.
    message_id : int	//! Unique message identifier

def from_JV ( var v:JsonValue?; t:messageid ) : messageid
    var q : messageid
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:messageid? ) : messageid?
    return null if v==null
    var q = new messageid
    *q <- from_JV(v,type<messageid>)
    return q

struct messageentity
    //!  This object represents one special entity in a text message. For example, hashtags, usernames, URLs, etc.
    _type : string	//! Type of the entity. Currently, can be “mention” (@username), “hashtag” (#hashtag), “cashtag” ($USD), “bot_command” (/start@jobs_bot), “url” (https://telegram.org), “email” (do-not-reply@telegram.org), “phone_number” (+1-212-555-0123), “bold” (bold text), “italic” (italic text), “underline” (underlined text), “strikethrough” (strikethrough text), “spoiler” (spoiler message), “code” (monowidth string), “pre” (monowidth block), “text_link” (for clickable text URLs), “text_mention” (for users without usernames), “custom_emoji” (for inline custom emoji stickers)
    offset : int	//! Offset in UTF-16 code units to the start of the entity
    length : int	//! Length of the entity in UTF-16 code units
    [[optional]] url : string	//! Optional. For “text_link” only, URL that will be opened after user taps on the text
    [[optional]] user : user?	//! Optional. For “text_mention” only, the mentioned user
    [[optional]] language : string	//! Optional. For “pre” only, the programming language of the entity text
    [[optional]] custom_emoji_id : string	//! Optional. For “custom_emoji” only, unique identifier of the custom emoji. Use getCustomEmojiStickers to get full information about the sticker

def from_JV ( var v:JsonValue?; t:messageentity ) : messageentity
    var q : messageentity
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.offset, _::from_JV((v as _object)["offset"], type<int>))
    move_to_ref(q.length, _::from_JV((v as _object)["length"], type<int>))
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user?>))
    move_to_ref(q.language, _::from_JV((v as _object)["language"], type<string>))
    move_to_ref(q.custom_emoji_id, _::from_JV((v as _object)["custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:messageentity? ) : messageentity?
    return null if v==null
    var q = new messageentity
    *q <- from_JV(v,type<messageentity>)
    return q

struct photosize
    //!  This object represents one size of a photo or a file / sticker thumbnail.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    width : int	//! Photo width
    height : int	//! Photo height
    [[optional]] file_size : int	//! Optional. File size in bytes

def from_JV ( var v:JsonValue?; t:photosize ) : photosize
    var q : photosize
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:photosize? ) : photosize?
    return null if v==null
    var q = new photosize
    *q <- from_JV(v,type<photosize>)
    return q

struct animation
    //!  This object represents an animation file (GIF or H.264/MPEG-4 AVC video without sound).
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    width : int	//! Video width as defined by sender
    height : int	//! Video height as defined by sender
    duration : int	//! Duration of the video in seconds as defined by sender
    [[optional]] thumbnail : photosize?	//! Optional. Animation thumbnail as defined by sender
    [[optional]] file_name : string	//! Optional. Original animation filename as defined by sender
    [[optional]] mime_type : string	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

def from_JV ( var v:JsonValue?; t:animation ) : animation
    var q : animation
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    move_to_ref(q.file_name, _::from_JV((v as _object)["file_name"], type<string>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:animation? ) : animation?
    return null if v==null
    var q = new animation
    *q <- from_JV(v,type<animation>)
    return q

struct audio
    //!  This object represents an audio file to be treated as music by the Telegram clients.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    duration : int	//! Duration of the audio in seconds as defined by sender
    [[optional]] performer : string	//! Optional. Performer of the audio as defined by sender or by audio tags
    [[optional]] title : string	//! Optional. Title of the audio as defined by sender or by audio tags
    [[optional]] file_name : string	//! Optional. Original filename as defined by sender
    [[optional]] mime_type : string	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.
    [[optional]] thumbnail : photosize?	//! Optional. Thumbnail of the album cover to which the music file belongs

def from_JV ( var v:JsonValue?; t:audio ) : audio
    var q : audio
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.performer, _::from_JV((v as _object)["performer"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.file_name, _::from_JV((v as _object)["file_name"], type<string>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    return <- q

def from_JV ( var v:JsonValue?; t:audio? ) : audio?
    return null if v==null
    var q = new audio
    *q <- from_JV(v,type<audio>)
    return q

struct document
    //!  This object represents a general file (as opposed to photos, voice messages and audio files).
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    [[optional]] thumbnail : photosize?	//! Optional. Document thumbnail as defined by sender
    [[optional]] file_name : string	//! Optional. Original filename as defined by sender
    [[optional]] mime_type : string	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

def from_JV ( var v:JsonValue?; t:document ) : document
    var q : document
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    move_to_ref(q.file_name, _::from_JV((v as _object)["file_name"], type<string>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:document? ) : document?
    return null if v==null
    var q = new document
    *q <- from_JV(v,type<document>)
    return q

struct video
    //!  This object represents a video file.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    width : int	//! Video width as defined by sender
    height : int	//! Video height as defined by sender
    duration : int	//! Duration of the video in seconds as defined by sender
    [[optional]] thumbnail : photosize?	//! Optional. Video thumbnail
    [[optional]] file_name : string	//! Optional. Original filename as defined by sender
    [[optional]] mime_type : string	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

def from_JV ( var v:JsonValue?; t:video ) : video
    var q : video
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    move_to_ref(q.file_name, _::from_JV((v as _object)["file_name"], type<string>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:video? ) : video?
    return null if v==null
    var q = new video
    *q <- from_JV(v,type<video>)
    return q

struct videonote
    //!  This object represents a video message (available in Telegram apps as of v.4.0).
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    length : int	//! Video width and height (diameter of the video message) as defined by sender
    duration : int	//! Duration of the video in seconds as defined by sender
    [[optional]] thumbnail : photosize?	//! Optional. Video thumbnail
    [[optional]] file_size : int	//! Optional. File size in bytes

def from_JV ( var v:JsonValue?; t:videonote ) : videonote
    var q : videonote
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.length, _::from_JV((v as _object)["length"], type<int>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:videonote? ) : videonote?
    return null if v==null
    var q = new videonote
    *q <- from_JV(v,type<videonote>)
    return q

struct voice
    //!  This object represents a voice note.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    duration : int	//! Duration of the audio in seconds as defined by sender
    [[optional]] mime_type : string	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

def from_JV ( var v:JsonValue?; t:voice ) : voice
    var q : voice
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:voice? ) : voice?
    return null if v==null
    var q = new voice
    *q <- from_JV(v,type<voice>)
    return q

struct contact
    //!  This object represents a phone contact.
    phone_number : string	//! Contact's phone number
    first_name : string	//! Contact's first name
    [[optional]] last_name : string	//! Optional. Contact's last name
    [[optional]] user_id : int	//! Optional. Contact's user identifier in Telegram. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] vcard : string	//! Optional. Additional data about the contact in the form of a vCard

def from_JV ( var v:JsonValue?; t:contact ) : contact
    var q : contact
    move_to_ref(q.phone_number, _::from_JV((v as _object)["phone_number"], type<string>))
    move_to_ref(q.first_name, _::from_JV((v as _object)["first_name"], type<string>))
    move_to_ref(q.last_name, _::from_JV((v as _object)["last_name"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.vcard, _::from_JV((v as _object)["vcard"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:contact? ) : contact?
    return null if v==null
    var q = new contact
    *q <- from_JV(v,type<contact>)
    return q

struct dice
    //!  This object represents an animated emoji that displays a random value.
    emoji : string	//! Emoji on which the dice throw animation is based
    value : int	//! Value of the dice, 1-6 for “”, “” and “” base emoji, 1-5 for “” and “” base emoji, 1-64 for “” base emoji

def from_JV ( var v:JsonValue?; t:dice ) : dice
    var q : dice
    move_to_ref(q.emoji, _::from_JV((v as _object)["emoji"], type<string>))
    move_to_ref(q.value, _::from_JV((v as _object)["value"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:dice? ) : dice?
    return null if v==null
    var q = new dice
    *q <- from_JV(v,type<dice>)
    return q

struct polloption
    //!  This object contains information about one answer option in a poll.
    text : string	//! Option text, 1-100 characters
    voter_count : int	//! Number of users that voted for this option

def from_JV ( var v:JsonValue?; t:polloption ) : polloption
    var q : polloption
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.voter_count, _::from_JV((v as _object)["voter_count"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:polloption? ) : polloption?
    return null if v==null
    var q = new polloption
    *q <- from_JV(v,type<polloption>)
    return q

struct pollanswer
    //!  This object represents an answer of a user in a non-anonymous poll.
    poll_id : string	//! Unique poll identifier
    user : user	//! The user, who changed the answer to the poll
    option_ids : array<int>	//! 0-based identifiers of answer options, chosen by the user. May be empty if the user retracted their vote.

def from_JV ( var v:JsonValue?; t:pollanswer ) : pollanswer
    var q : pollanswer
    move_to_ref(q.poll_id, _::from_JV((v as _object)["poll_id"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    move_to_ref(q.option_ids, _::from_JV((v as _object)["option_ids"], type<array<int>>))
    return <- q

def from_JV ( var v:JsonValue?; t:pollanswer? ) : pollanswer?
    return null if v==null
    var q = new pollanswer
    *q <- from_JV(v,type<pollanswer>)
    return q

struct poll
    //!  This object contains information about a poll.
    id : string	//! Unique poll identifier
    question : string	//! Poll question, 1-300 characters
    _options : array<polloption>	//! List of poll options
    total_voter_count : int	//! Total number of users that voted in the poll
    is_closed : bool	//! True, if the poll is closed
    is_anonymous : bool	//! True, if the poll is anonymous
    _type : string	//! Poll type, currently can be “regular” or “quiz”
    allows_multiple_answers : bool	//! True, if the poll allows multiple answers
    [[optional]] correct_option_id : int	//! Optional. 0-based identifier of the correct answer option. Available only for polls in the quiz mode, which are closed, or was sent (not forwarded) by the bot or to the private chat with the bot.
    [[optional]] explanation : string	//! Optional. Text that is shown when a user chooses an incorrect answer or taps on the lamp icon in a quiz-style poll, 0-200 characters
    [[optional]] explanation_entities : array<messageentity>	//! Optional. Special entities like usernames, URLs, bot commands, etc. that appear in the explanation
    [[optional]] open_period : int	//! Optional. Amount of time in seconds the poll will be active after creation
    [[optional]] close_date : int	//! Optional. Point in time (Unix timestamp) when the poll will be automatically closed

def from_JV ( var v:JsonValue?; t:poll ) : poll
    var q : poll
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.question, _::from_JV((v as _object)["question"], type<string>))
    move_to_ref(q._options, _::from_JV((v as _object)["_options"], type<array<polloption>>))
    move_to_ref(q.total_voter_count, _::from_JV((v as _object)["total_voter_count"], type<int>))
    move_to_ref(q.is_closed, _::from_JV((v as _object)["is_closed"], type<bool>))
    move_to_ref(q.is_anonymous, _::from_JV((v as _object)["is_anonymous"], type<bool>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.allows_multiple_answers, _::from_JV((v as _object)["allows_multiple_answers"], type<bool>))
    move_to_ref(q.correct_option_id, _::from_JV((v as _object)["correct_option_id"], type<int>))
    move_to_ref(q.explanation, _::from_JV((v as _object)["explanation"], type<string>))
    move_to_ref(q.explanation_entities, _::from_JV((v as _object)["explanation_entities"], type<array<messageentity>>))
    move_to_ref(q.open_period, _::from_JV((v as _object)["open_period"], type<int>))
    move_to_ref(q.close_date, _::from_JV((v as _object)["close_date"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:poll? ) : poll?
    return null if v==null
    var q = new poll
    *q <- from_JV(v,type<poll>)
    return q

struct location
    //!  This object represents a point on the map.
    longitude : float	//! Longitude as defined by sender
    latitude : float	//! Latitude as defined by sender
    [[optional]] horizontal_accuracy : float	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int	//! Optional. Time relative to the message sending date, during which the location can be updated; in seconds. For active live locations only.
    [[optional]] heading : int	//! Optional. The direction in which user is moving, in degrees; 1-360. For active live locations only.
    [[optional]] proximity_alert_radius : int	//! Optional. The maximum distance for proximity alerts about approaching another chat member, in meters. For sent live locations only.

def from_JV ( var v:JsonValue?; t:location ) : location
    var q : location
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.horizontal_accuracy, _::from_JV((v as _object)["horizontal_accuracy"], type<float>))
    move_to_ref(q.live_period, _::from_JV((v as _object)["live_period"], type<int>))
    move_to_ref(q.heading, _::from_JV((v as _object)["heading"], type<int>))
    move_to_ref(q.proximity_alert_radius, _::from_JV((v as _object)["proximity_alert_radius"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:location? ) : location?
    return null if v==null
    var q = new location
    *q <- from_JV(v,type<location>)
    return q

struct venue
    //!  This object represents a venue.
    location : location	//! Venue location. Can't be a live location
    title : string	//! Name of the venue
    address : string	//! Address of the venue
    [[optional]] foursquare_id : string	//! Optional. Foursquare identifier of the venue
    [[optional]] foursquare_type : string	//! Optional. Foursquare type of the venue. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string	//! Optional. Google Places type of the venue. (See supported types.)

def from_JV ( var v:JsonValue?; t:venue ) : venue
    var q : venue
    move_to_ref(q.location, _::from_JV((v as _object)["location"], type<location>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.address, _::from_JV((v as _object)["address"], type<string>))
    move_to_ref(q.foursquare_id, _::from_JV((v as _object)["foursquare_id"], type<string>))
    move_to_ref(q.foursquare_type, _::from_JV((v as _object)["foursquare_type"], type<string>))
    move_to_ref(q.google_place_id, _::from_JV((v as _object)["google_place_id"], type<string>))
    move_to_ref(q.google_place_type, _::from_JV((v as _object)["google_place_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:venue? ) : venue?
    return null if v==null
    var q = new venue
    *q <- from_JV(v,type<venue>)
    return q

struct webappdata
    //!  Describes data sent from a Web App to the bot.
    data : string	//! The data. Be aware that a bad client can send arbitrary data in this field.
    button_text : string	//! Text of the web_app keyboard button from which the Web App was opened. Be aware that a bad client can send arbitrary data in this field.

def from_JV ( var v:JsonValue?; t:webappdata ) : webappdata
    var q : webappdata
    move_to_ref(q.data, _::from_JV((v as _object)["data"], type<string>))
    move_to_ref(q.button_text, _::from_JV((v as _object)["button_text"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:webappdata? ) : webappdata?
    return null if v==null
    var q = new webappdata
    *q <- from_JV(v,type<webappdata>)
    return q

struct proximityalerttriggered
    //!  This object represents the content of a service message, sent whenever a user in the chat triggers a proximity alert set by another user.
    traveler : user	//! User that triggered the alert
    watcher : user	//! User that set the alert
    distance : int	//! The distance between the users

def from_JV ( var v:JsonValue?; t:proximityalerttriggered ) : proximityalerttriggered
    var q : proximityalerttriggered
    move_to_ref(q.traveler, _::from_JV((v as _object)["traveler"], type<user>))
    move_to_ref(q.watcher, _::from_JV((v as _object)["watcher"], type<user>))
    move_to_ref(q.distance, _::from_JV((v as _object)["distance"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:proximityalerttriggered? ) : proximityalerttriggered?
    return null if v==null
    var q = new proximityalerttriggered
    *q <- from_JV(v,type<proximityalerttriggered>)
    return q

struct messageautodeletetimerchanged
    //!  This object represents a service message about a change in auto-delete timer settings.
    message_auto_delete_time : int	//! New auto-delete time for messages in the chat; in seconds

def from_JV ( var v:JsonValue?; t:messageautodeletetimerchanged ) : messageautodeletetimerchanged
    var q : messageautodeletetimerchanged
    move_to_ref(q.message_auto_delete_time, _::from_JV((v as _object)["message_auto_delete_time"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:messageautodeletetimerchanged? ) : messageautodeletetimerchanged?
    return null if v==null
    var q = new messageautodeletetimerchanged
    *q <- from_JV(v,type<messageautodeletetimerchanged>)
    return q

struct forumtopiccreated
    //!  This object represents a service message about a new forum topic created in the chat.
    name : string	//! Name of the topic
    icon_color : int	//! Color of the topic icon in RGB format
    [[optional]] icon_custom_emoji_id : string	//! Optional. Unique identifier of the custom emoji shown as the topic icon

def from_JV ( var v:JsonValue?; t:forumtopiccreated ) : forumtopiccreated
    var q : forumtopiccreated
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.icon_color, _::from_JV((v as _object)["icon_color"], type<int>))
    move_to_ref(q.icon_custom_emoji_id, _::from_JV((v as _object)["icon_custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:forumtopiccreated? ) : forumtopiccreated?
    return null if v==null
    var q = new forumtopiccreated
    *q <- from_JV(v,type<forumtopiccreated>)
    return q

struct forumtopicclosed
    //!  This object represents a service message about a forum topic closed in the chat. Currently holds no information.
    object_forumtopicclosed_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:forumtopicclosed ) : forumtopicclosed
    var q : forumtopicclosed
    return <- q

def from_JV ( var v:JsonValue?; t:forumtopicclosed? ) : forumtopicclosed?
    return null if v==null
    var q = new forumtopicclosed
    *q <- from_JV(v,type<forumtopicclosed>)
    return q

struct forumtopicedited
    //!  This object represents a service message about an edited forum topic.
    [[optional]] name : string	//! Optional. New name of the topic, if it was edited
    [[optional]] icon_custom_emoji_id : string	//! Optional. New identifier of the custom emoji shown as the topic icon, if it was edited; an empty string if the icon was removed

def from_JV ( var v:JsonValue?; t:forumtopicedited ) : forumtopicedited
    var q : forumtopicedited
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.icon_custom_emoji_id, _::from_JV((v as _object)["icon_custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:forumtopicedited? ) : forumtopicedited?
    return null if v==null
    var q = new forumtopicedited
    *q <- from_JV(v,type<forumtopicedited>)
    return q

struct forumtopicreopened
    //!  This object represents a service message about a forum topic reopened in the chat. Currently holds no information.
    object_forumtopicreopened_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:forumtopicreopened ) : forumtopicreopened
    var q : forumtopicreopened
    return <- q

def from_JV ( var v:JsonValue?; t:forumtopicreopened? ) : forumtopicreopened?
    return null if v==null
    var q = new forumtopicreopened
    *q <- from_JV(v,type<forumtopicreopened>)
    return q

struct generalforumtopichidden
    //!  This object represents a service message about General forum topic hidden in the chat. Currently holds no information.
    object_generalforumtopichidden_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:generalforumtopichidden ) : generalforumtopichidden
    var q : generalforumtopichidden
    return <- q

def from_JV ( var v:JsonValue?; t:generalforumtopichidden? ) : generalforumtopichidden?
    return null if v==null
    var q = new generalforumtopichidden
    *q <- from_JV(v,type<generalforumtopichidden>)
    return q

struct generalforumtopicunhidden
    //!  This object represents a service message about General forum topic unhidden in the chat. Currently holds no information.
    object_generalforumtopicunhidden_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:generalforumtopicunhidden ) : generalforumtopicunhidden
    var q : generalforumtopicunhidden
    return <- q

def from_JV ( var v:JsonValue?; t:generalforumtopicunhidden? ) : generalforumtopicunhidden?
    return null if v==null
    var q = new generalforumtopicunhidden
    *q <- from_JV(v,type<generalforumtopicunhidden>)
    return q

struct usershared
    //!  This object contains information about the user whose identifier was shared with the bot using a KeyboardButtonRequestUser button.
    request_id : int	//! Identifier of the request
    user_id : int	//! Identifier of the shared user. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier. The bot may not have access to the user and could be unable to use this identifier, unless the user is already known to the bot by some other means.

def from_JV ( var v:JsonValue?; t:usershared ) : usershared
    var q : usershared
    move_to_ref(q.request_id, _::from_JV((v as _object)["request_id"], type<int>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:usershared? ) : usershared?
    return null if v==null
    var q = new usershared
    *q <- from_JV(v,type<usershared>)
    return q

struct chatshared
    //!  This object contains information about the chat whose identifier was shared with the bot using a KeyboardButtonRequestChat button.
    request_id : int	//! Identifier of the request
    chat_id : int	//! Identifier of the shared chat. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier. The bot may not have access to the chat and could be unable to use this identifier, unless the chat is already known to the bot by some other means.

def from_JV ( var v:JsonValue?; t:chatshared ) : chatshared
    var q : chatshared
    move_to_ref(q.request_id, _::from_JV((v as _object)["request_id"], type<int>))
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatshared? ) : chatshared?
    return null if v==null
    var q = new chatshared
    *q <- from_JV(v,type<chatshared>)
    return q

struct writeaccessallowed
    //!  This object represents a service message about a user allowing a bot added to the attachment menu to write messages. Currently holds no information.
    object_writeaccessallowed_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:writeaccessallowed ) : writeaccessallowed
    var q : writeaccessallowed
    return <- q

def from_JV ( var v:JsonValue?; t:writeaccessallowed? ) : writeaccessallowed?
    return null if v==null
    var q = new writeaccessallowed
    *q <- from_JV(v,type<writeaccessallowed>)
    return q

struct videochatscheduled
    //!  This object represents a service message about a video chat scheduled in the chat.
    start_date : int	//! Point in time (Unix timestamp) when the video chat is supposed to be started by a chat administrator

def from_JV ( var v:JsonValue?; t:videochatscheduled ) : videochatscheduled
    var q : videochatscheduled
    move_to_ref(q.start_date, _::from_JV((v as _object)["start_date"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:videochatscheduled? ) : videochatscheduled?
    return null if v==null
    var q = new videochatscheduled
    *q <- from_JV(v,type<videochatscheduled>)
    return q

struct videochatstarted
    //!  This object represents a service message about a video chat started in the chat. Currently holds no information.
    object_videochatstarted_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:videochatstarted ) : videochatstarted
    var q : videochatstarted
    return <- q

def from_JV ( var v:JsonValue?; t:videochatstarted? ) : videochatstarted?
    return null if v==null
    var q = new videochatstarted
    *q <- from_JV(v,type<videochatstarted>)
    return q

struct videochatended
    //!  This object represents a service message about a video chat ended in the chat.
    duration : int	//! Video chat duration in seconds

def from_JV ( var v:JsonValue?; t:videochatended ) : videochatended
    var q : videochatended
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:videochatended? ) : videochatended?
    return null if v==null
    var q = new videochatended
    *q <- from_JV(v,type<videochatended>)
    return q

struct videochatparticipantsinvited
    //!  This object represents a service message about new members invited to a video chat.
    users : array<user>	//! New members that were invited to the video chat

def from_JV ( var v:JsonValue?; t:videochatparticipantsinvited ) : videochatparticipantsinvited
    var q : videochatparticipantsinvited
    move_to_ref(q.users, _::from_JV((v as _object)["users"], type<array<user>>))
    return <- q

def from_JV ( var v:JsonValue?; t:videochatparticipantsinvited? ) : videochatparticipantsinvited?
    return null if v==null
    var q = new videochatparticipantsinvited
    *q <- from_JV(v,type<videochatparticipantsinvited>)
    return q

struct userprofilephotos
    //!  This object represent a user's profile pictures.
    total_count : int	//! Total number of profile pictures the target user has
    photos : array<array<photosize>>	//! Requested profile pictures (in up to 4 sizes each)

def from_JV ( var v:JsonValue?; t:userprofilephotos ) : userprofilephotos
    var q : userprofilephotos
    move_to_ref(q.total_count, _::from_JV((v as _object)["total_count"], type<int>))
    move_to_ref(q.photos, _::from_JV((v as _object)["photos"], type<array<array<photosize>>>))
    return <- q

def from_JV ( var v:JsonValue?; t:userprofilephotos? ) : userprofilephotos?
    return null if v==null
    var q = new userprofilephotos
    *q <- from_JV(v,type<userprofilephotos>)
    return q

struct file
    //!  This object represents a file ready to be downloaded. The file can be downloaded via the link https://api.telegram.org/file/bot<token>/<file_path>. It is guaranteed that the link will be valid for at least 1 hour. When the link expires, a new one can be requested by calling getFile.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    [[optional]] file_size : int	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.
    [[optional]] file_path : string	//! Optional. File path. Use https://api.telegram.org/file/bot<token>/<file_path> to get the file.

def from_JV ( var v:JsonValue?; t:file ) : file
    var q : file
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    move_to_ref(q.file_path, _::from_JV((v as _object)["file_path"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:file? ) : file?
    return null if v==null
    var q = new file
    *q <- from_JV(v,type<file>)
    return q

struct webappinfo
    //!  Describes a Web App.
    url : string	//! An HTTPS URL of a Web App to be opened with additional data as specified in Initializing Web Apps

def from_JV ( var v:JsonValue?; t:webappinfo ) : webappinfo
    var q : webappinfo
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:webappinfo? ) : webappinfo?
    return null if v==null
    var q = new webappinfo
    *q <- from_JV(v,type<webappinfo>)
    return q

struct replykeyboardmarkup
    //!  This object represents a custom keyboard with reply options (see Introduction to bots for details and examples).
    keyboard : array<array<keyboardbutton>>	//! Array of button rows, each represented by an Array of KeyboardButton objects
    [[optional]] is_persistent : bool	//! Optional. Requests clients to always show the keyboard when the regular keyboard is hidden. Defaults to false, in which case the custom keyboard can be hidden and opened with a keyboard icon.
    [[optional]] resize_keyboard : bool	//! Optional. Requests clients to resize the keyboard vertically for optimal fit (e.g., make the keyboard smaller if there are just two rows of buttons). Defaults to false, in which case the custom keyboard is always of the same height as the app's standard keyboard.
    [[optional]] one_time_keyboard : bool	//! Optional. Requests clients to hide the keyboard as soon as it's been used. The keyboard will still be available, but clients will automatically display the usual letter-keyboard in the chat - the user can press a special button in the input field to see the custom keyboard again. Defaults to false.
    [[optional]] input_field_placeholder : string	//! Optional. The placeholder to be shown in the input field when the keyboard is active; 1-64 characters
    [[optional]] selective : bool	//! Optional. Use this parameter if you want to show the keyboard to specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.Example: A user requests to change the bot's language, bot replies to the request with a keyboard to select the new language. Other users in the group don't see the keyboard.

def from_JV ( var v:JsonValue?; t:replykeyboardmarkup ) : replykeyboardmarkup
    var q : replykeyboardmarkup
    move_to_ref(q.keyboard, _::from_JV((v as _object)["keyboard"], type<array<array<keyboardbutton>>>))
    move_to_ref(q.is_persistent, _::from_JV((v as _object)["is_persistent"], type<bool>))
    move_to_ref(q.resize_keyboard, _::from_JV((v as _object)["resize_keyboard"], type<bool>))
    move_to_ref(q.one_time_keyboard, _::from_JV((v as _object)["one_time_keyboard"], type<bool>))
    move_to_ref(q.input_field_placeholder, _::from_JV((v as _object)["input_field_placeholder"], type<string>))
    move_to_ref(q.selective, _::from_JV((v as _object)["selective"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:replykeyboardmarkup? ) : replykeyboardmarkup?
    return null if v==null
    var q = new replykeyboardmarkup
    *q <- from_JV(v,type<replykeyboardmarkup>)
    return q

struct keyboardbutton
    //!  This object represents one button of the reply keyboard. For simple text buttons, String can be used instead of this object to specify the button text. The optional fields web_app, request_user, request_chat, request_contact, request_location, and request_poll are mutually exclusive.
    text : string	//! Text of the button. If none of the optional fields are used, it will be sent as a message when the button is pressed
    [[optional]] request_user : keyboardbuttonrequestuser?	//! Optional. If specified, pressing the button will open a list of suitable users. Tapping on any user will send their identifier to the bot in a “user_shared” service message. Available in private chats only.
    [[optional]] request_chat : keyboardbuttonrequestchat?	//! Optional. If specified, pressing the button will open a list of suitable chats. Tapping on a chat will send its identifier to the bot in a “chat_shared” service message. Available in private chats only.
    [[optional]] request_contact : bool	//! Optional. If True, the user's phone number will be sent as a contact when the button is pressed. Available in private chats only.
    [[optional]] request_location : bool	//! Optional. If True, the user's current location will be sent when the button is pressed. Available in private chats only.
    [[optional]] request_poll : keyboardbuttonpolltype?	//! Optional. If specified, the user will be asked to create a poll and send it to the bot when the button is pressed. Available in private chats only.
    [[optional]] web_app : webappinfo?	//! Optional. If specified, the described Web App will be launched when the button is pressed. The Web App will be able to send a “web_app_data” service message. Available in private chats only.

def from_JV ( var v:JsonValue?; t:keyboardbutton ) : keyboardbutton
    var q : keyboardbutton
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.request_user, _::from_JV((v as _object)["request_user"], type<keyboardbuttonrequestuser?>))
    move_to_ref(q.request_chat, _::from_JV((v as _object)["request_chat"], type<keyboardbuttonrequestchat?>))
    move_to_ref(q.request_contact, _::from_JV((v as _object)["request_contact"], type<bool>))
    move_to_ref(q.request_location, _::from_JV((v as _object)["request_location"], type<bool>))
    move_to_ref(q.request_poll, _::from_JV((v as _object)["request_poll"], type<keyboardbuttonpolltype?>))
    move_to_ref(q.web_app, _::from_JV((v as _object)["web_app"], type<webappinfo?>))
    return <- q

def from_JV ( var v:JsonValue?; t:keyboardbutton? ) : keyboardbutton?
    return null if v==null
    var q = new keyboardbutton
    *q <- from_JV(v,type<keyboardbutton>)
    return q

struct keyboardbuttonrequestuser
    //!  This object defines the criteria used to request a suitable user. The identifier of the selected user will be shared with the bot when the corresponding button is pressed. More about requesting users »
    request_id : int	//! Signed 32-bit identifier of the request, which will be received back in the UserShared object. Must be unique within the message
    [[optional]] user_is_bot : bool	//! Optional. Pass True to request a bot, pass False to request a regular user. If not specified, no additional restrictions are applied.
    [[optional]] user_is_premium : bool	//! Optional. Pass True to request a premium user, pass False to request a non-premium user. If not specified, no additional restrictions are applied.

def from_JV ( var v:JsonValue?; t:keyboardbuttonrequestuser ) : keyboardbuttonrequestuser
    var q : keyboardbuttonrequestuser
    move_to_ref(q.request_id, _::from_JV((v as _object)["request_id"], type<int>))
    move_to_ref(q.user_is_bot, _::from_JV((v as _object)["user_is_bot"], type<bool>))
    move_to_ref(q.user_is_premium, _::from_JV((v as _object)["user_is_premium"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:keyboardbuttonrequestuser? ) : keyboardbuttonrequestuser?
    return null if v==null
    var q = new keyboardbuttonrequestuser
    *q <- from_JV(v,type<keyboardbuttonrequestuser>)
    return q

struct keyboardbuttonrequestchat
    //!  This object defines the criteria used to request a suitable chat. The identifier of the selected chat will be shared with the bot when the corresponding button is pressed. More about requesting chats »
    request_id : int	//! Signed 32-bit identifier of the request, which will be received back in the ChatShared object. Must be unique within the message
    chat_is_channel : bool	//! Pass True to request a channel chat, pass False to request a group or a supergroup chat.
    [[optional]] chat_is_forum : bool	//! Optional. Pass True to request a forum supergroup, pass False to request a non-forum chat. If not specified, no additional restrictions are applied.
    [[optional]] chat_has_username : bool	//! Optional. Pass True to request a supergroup or a channel with a username, pass False to request a chat without a username. If not specified, no additional restrictions are applied.
    [[optional]] chat_is_created : bool	//! Optional. Pass True to request a chat owned by the user. Otherwise, no additional restrictions are applied.
    [[optional]] user_administrator_rights : chatadministratorrights?	//! Optional. A JSON-serialized object listing the required administrator rights of the user in the chat. The rights must be a superset of bot_administrator_rights. If not specified, no additional restrictions are applied.
    [[optional]] bot_administrator_rights : chatadministratorrights?	//! Optional. A JSON-serialized object listing the required administrator rights of the bot in the chat. The rights must be a subset of user_administrator_rights. If not specified, no additional restrictions are applied.
    [[optional]] bot_is_member : bool	//! Optional. Pass True to request a chat with the bot as a member. Otherwise, no additional restrictions are applied.

def from_JV ( var v:JsonValue?; t:keyboardbuttonrequestchat ) : keyboardbuttonrequestchat
    var q : keyboardbuttonrequestchat
    move_to_ref(q.request_id, _::from_JV((v as _object)["request_id"], type<int>))
    move_to_ref(q.chat_is_channel, _::from_JV((v as _object)["chat_is_channel"], type<bool>))
    move_to_ref(q.chat_is_forum, _::from_JV((v as _object)["chat_is_forum"], type<bool>))
    move_to_ref(q.chat_has_username, _::from_JV((v as _object)["chat_has_username"], type<bool>))
    move_to_ref(q.chat_is_created, _::from_JV((v as _object)["chat_is_created"], type<bool>))
    move_to_ref(q.user_administrator_rights, _::from_JV((v as _object)["user_administrator_rights"], type<chatadministratorrights?>))
    move_to_ref(q.bot_administrator_rights, _::from_JV((v as _object)["bot_administrator_rights"], type<chatadministratorrights?>))
    move_to_ref(q.bot_is_member, _::from_JV((v as _object)["bot_is_member"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:keyboardbuttonrequestchat? ) : keyboardbuttonrequestchat?
    return null if v==null
    var q = new keyboardbuttonrequestchat
    *q <- from_JV(v,type<keyboardbuttonrequestchat>)
    return q

struct keyboardbuttonpolltype
    //!  This object represents type of a poll, which is allowed to be created and sent when the corresponding button is pressed.
    [[optional]] _type : string	//! Optional. If quiz is passed, the user will be allowed to create only polls in the quiz mode. If regular is passed, only regular polls will be allowed. Otherwise, the user will be allowed to create a poll of any type.

def from_JV ( var v:JsonValue?; t:keyboardbuttonpolltype ) : keyboardbuttonpolltype
    var q : keyboardbuttonpolltype
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:keyboardbuttonpolltype? ) : keyboardbuttonpolltype?
    return null if v==null
    var q = new keyboardbuttonpolltype
    *q <- from_JV(v,type<keyboardbuttonpolltype>)
    return q

struct replykeyboardremove
    //!  Upon receiving a message with this object, Telegram clients will remove the current custom keyboard and display the default letter-keyboard. By default, custom keyboards are displayed until a new keyboard is sent by a bot. An exception is made for one-time keyboards that are hidden immediately after the user presses a button (see ReplyKeyboardMarkup).
    remove_keyboard : bool	//! Requests clients to remove the custom keyboard (user will not be able to summon this keyboard; if you want to hide the keyboard from sight but keep it accessible, use one_time_keyboard in ReplyKeyboardMarkup)
    [[optional]] selective : bool	//! Optional. Use this parameter if you want to remove the keyboard for specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.Example: A user votes in a poll, bot returns confirmation message in reply to the vote and removes the keyboard for that user, while still showing the keyboard with poll options to users who haven't voted yet.

def from_JV ( var v:JsonValue?; t:replykeyboardremove ) : replykeyboardremove
    var q : replykeyboardremove
    move_to_ref(q.remove_keyboard, _::from_JV((v as _object)["remove_keyboard"], type<bool>))
    move_to_ref(q.selective, _::from_JV((v as _object)["selective"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:replykeyboardremove? ) : replykeyboardremove?
    return null if v==null
    var q = new replykeyboardremove
    *q <- from_JV(v,type<replykeyboardremove>)
    return q

struct inlinekeyboardmarkup
    //!  This object represents an inline keyboard that appears right next to the message it belongs to.
    inline_keyboard : array<array<inlinekeyboardbutton>>	//! Array of button rows, each represented by an Array of InlineKeyboardButton objects

def from_JV ( var v:JsonValue?; t:inlinekeyboardmarkup ) : inlinekeyboardmarkup
    var q : inlinekeyboardmarkup
    move_to_ref(q.inline_keyboard, _::from_JV((v as _object)["inline_keyboard"], type<array<array<inlinekeyboardbutton>>>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinekeyboardmarkup? ) : inlinekeyboardmarkup?
    return null if v==null
    var q = new inlinekeyboardmarkup
    *q <- from_JV(v,type<inlinekeyboardmarkup>)
    return q

struct inlinekeyboardbutton
    //!  This object represents one button of an inline keyboard. You must use exactly one of the optional fields.
    text : string	//! Label text on the button
    [[optional]] url : string	//! Optional. HTTP or tg:// URL to be opened when the button is pressed. Links tg://user?id=<user_id> can be used to mention a user by their ID without using a username, if this is allowed by their privacy settings.
    [[optional]] callback_data : string	//! Optional. Data to be sent in a callback query to the bot when button is pressed, 1-64 bytes
    [[optional]] web_app : webappinfo?	//! Optional. Description of the Web App that will be launched when the user presses the button. The Web App will be able to send an arbitrary message on behalf of the user using the method answerWebAppQuery. Available only in private chats between a user and the bot.
    [[optional]] login_url : loginurl?	//! Optional. An HTTPS URL used to automatically authorize the user. Can be used as a replacement for the Telegram Login Widget.
    [[optional]] switch_inline_query : string	//! Optional. If set, pressing the button will prompt the user to select one of their chats, open that chat and insert the bot's username and the specified inline query in the input field. May be empty, in which case just the bot's username will be inserted.Note: This offers an easy way for users to start using your bot in inline mode when they are currently in a private chat with it. Especially useful when combined with switch_pm… actions - in this case the user will be automatically returned to the chat they switched from, skipping the chat selection screen.
    [[optional]] switch_inline_query_current_chat : string	//! Optional. If set, pressing the button will insert the bot's username and the specified inline query in the current chat's input field. May be empty, in which case only the bot's username will be inserted. This offers a quick way for the user to open your bot in inline mode in the same chat - good for selecting something from multiple options.
    [[optional]] callback_game : callbackgame?	//! Optional. Description of the game that will be launched when the user presses the button.NOTE: This type of button must always be the first button in the first row.
    [[optional]] pay : bool	//! Optional. Specify True, to send a Pay button.NOTE: This type of button must always be the first button in the first row and can only be used in invoice messages.

def from_JV ( var v:JsonValue?; t:inlinekeyboardbutton ) : inlinekeyboardbutton
    var q : inlinekeyboardbutton
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.callback_data, _::from_JV((v as _object)["callback_data"], type<string>))
    move_to_ref(q.web_app, _::from_JV((v as _object)["web_app"], type<webappinfo?>))
    move_to_ref(q.login_url, _::from_JV((v as _object)["login_url"], type<loginurl?>))
    move_to_ref(q.switch_inline_query, _::from_JV((v as _object)["switch_inline_query"], type<string>))
    move_to_ref(q.switch_inline_query_current_chat, _::from_JV((v as _object)["switch_inline_query_current_chat"], type<string>))
    move_to_ref(q.callback_game, _::from_JV((v as _object)["callback_game"], type<callbackgame?>))
    move_to_ref(q.pay, _::from_JV((v as _object)["pay"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinekeyboardbutton? ) : inlinekeyboardbutton?
    return null if v==null
    var q = new inlinekeyboardbutton
    *q <- from_JV(v,type<inlinekeyboardbutton>)
    return q

struct loginurl
    //!  This object represents a parameter of the inline keyboard button used to automatically authorize a user. Serves as a great replacement for the Telegram Login Widget when the user is coming from Telegram. All the user needs to do is tap/click a button and confirm that they want to log in: Telegram apps support these buttons as of version 5.7.
    url : string	//! An HTTPS URL to be opened with user authorization data added to the query string when the button is pressed. If the user refuses to provide authorization data, the original URL without information about the user will be opened. The data added is the same as described in Receiving authorization data.NOTE: You must always check the hash of the received data to verify the authentication and the integrity of the data as described in Checking authorization.
    [[optional]] forward_text : string	//! Optional. New text of the button in forwarded messages.
    [[optional]] bot_username : string	//! Optional. Username of a bot, which will be used for user authorization. See Setting up a bot for more details. If not specified, the current bot's username will be assumed. The url's domain must be the same as the domain linked with the bot. See Linking your domain to the bot for more details.
    [[optional]] request_write_access : bool	//! Optional. Pass True to request the permission for your bot to send messages to the user.

def from_JV ( var v:JsonValue?; t:loginurl ) : loginurl
    var q : loginurl
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.forward_text, _::from_JV((v as _object)["forward_text"], type<string>))
    move_to_ref(q.bot_username, _::from_JV((v as _object)["bot_username"], type<string>))
    move_to_ref(q.request_write_access, _::from_JV((v as _object)["request_write_access"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:loginurl? ) : loginurl?
    return null if v==null
    var q = new loginurl
    *q <- from_JV(v,type<loginurl>)
    return q

struct callbackquery
    //!  This object represents an incoming callback query from a callback button in an inline keyboard. If the button that originated the query was attached to a message sent by the bot, the field message will be present. If the button was attached to a message sent via the bot (in inline mode), the field inline_message_id will be present. Exactly one of the fields data or game_short_name will be present.
    id : string	//! Unique identifier for this query
    from : user	//! Sender
    [[optional]] message : message?	//! Optional. Message with the callback button that originated the query. Note that message content and message date will not be available if the message is too old
    [[optional]] inline_message_id : string	//! Optional. Identifier of the message sent via the bot in inline mode, that originated the query.
    chat_instance : string	//! Global identifier, uniquely corresponding to the chat to which the message with the callback button was sent. Useful for high scores in games.
    [[optional]] data : string	//! Optional. Data associated with the callback button. Be aware that the message originated the query can contain no callback buttons with this data.
    [[optional]] game_short_name : string	//! Optional. Short name of a Game to be returned, serves as the unique identifier for the game

def from_JV ( var v:JsonValue?; t:callbackquery ) : callbackquery
    var q : callbackquery
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<message?>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.chat_instance, _::from_JV((v as _object)["chat_instance"], type<string>))
    move_to_ref(q.data, _::from_JV((v as _object)["data"], type<string>))
    move_to_ref(q.game_short_name, _::from_JV((v as _object)["game_short_name"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:callbackquery? ) : callbackquery?
    return null if v==null
    var q = new callbackquery
    *q <- from_JV(v,type<callbackquery>)
    return q

struct forcereply
    //!  Upon receiving a message with this object, Telegram clients will display a reply interface to the user (act as if the user has selected the bot's message and tapped 'Reply'). This can be extremely useful if you want to create user-friendly step-by-step interfaces without having to sacrifice privacy mode.
    force_reply : bool	//! Shows reply interface to the user, as if they manually selected the bot's message and tapped 'Reply'
    [[optional]] input_field_placeholder : string	//! Optional. The placeholder to be shown in the input field when the reply is active; 1-64 characters
    [[optional]] selective : bool	//! Optional. Use this parameter if you want to force reply from specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.

def from_JV ( var v:JsonValue?; t:forcereply ) : forcereply
    var q : forcereply
    move_to_ref(q.force_reply, _::from_JV((v as _object)["force_reply"], type<bool>))
    move_to_ref(q.input_field_placeholder, _::from_JV((v as _object)["input_field_placeholder"], type<string>))
    move_to_ref(q.selective, _::from_JV((v as _object)["selective"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:forcereply? ) : forcereply?
    return null if v==null
    var q = new forcereply
    *q <- from_JV(v,type<forcereply>)
    return q

struct chatphoto
    //!  This object represents a chat photo.
    small_file_id : string	//! File identifier of small (160x160) chat photo. This file_id can be used only for photo download and only for as long as the photo is not changed.
    small_file_unique_id : string	//! Unique file identifier of small (160x160) chat photo, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    big_file_id : string	//! File identifier of big (640x640) chat photo. This file_id can be used only for photo download and only for as long as the photo is not changed.
    big_file_unique_id : string	//! Unique file identifier of big (640x640) chat photo, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.

def from_JV ( var v:JsonValue?; t:chatphoto ) : chatphoto
    var q : chatphoto
    move_to_ref(q.small_file_id, _::from_JV((v as _object)["small_file_id"], type<string>))
    move_to_ref(q.small_file_unique_id, _::from_JV((v as _object)["small_file_unique_id"], type<string>))
    move_to_ref(q.big_file_id, _::from_JV((v as _object)["big_file_id"], type<string>))
    move_to_ref(q.big_file_unique_id, _::from_JV((v as _object)["big_file_unique_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatphoto? ) : chatphoto?
    return null if v==null
    var q = new chatphoto
    *q <- from_JV(v,type<chatphoto>)
    return q

struct chatinvitelink
    //!  Represents an invite link for a chat.
    invite_link : string	//! The invite link. If the link was created by another chat administrator, then the second part of the link will be replaced with “…”.
    creator : user	//! Creator of the link
    creates_join_request : bool	//! True, if users joining the chat via the link need to be approved by chat administrators
    is_primary : bool	//! True, if the link is primary
    is_revoked : bool	//! True, if the link is revoked
    [[optional]] name : string	//! Optional. Invite link name
    [[optional]] expire_date : int	//! Optional. Point in time (Unix timestamp) when the link will expire or has been expired
    [[optional]] member_limit : int	//! Optional. The maximum number of users that can be members of the chat simultaneously after joining the chat via this invite link; 1-99999
    [[optional]] pending_join_request_count : int	//! Optional. Number of pending join requests created using this link

def from_JV ( var v:JsonValue?; t:chatinvitelink ) : chatinvitelink
    var q : chatinvitelink
    move_to_ref(q.invite_link, _::from_JV((v as _object)["invite_link"], type<string>))
    move_to_ref(q.creator, _::from_JV((v as _object)["creator"], type<user>))
    move_to_ref(q.creates_join_request, _::from_JV((v as _object)["creates_join_request"], type<bool>))
    move_to_ref(q.is_primary, _::from_JV((v as _object)["is_primary"], type<bool>))
    move_to_ref(q.is_revoked, _::from_JV((v as _object)["is_revoked"], type<bool>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.expire_date, _::from_JV((v as _object)["expire_date"], type<int>))
    move_to_ref(q.member_limit, _::from_JV((v as _object)["member_limit"], type<int>))
    move_to_ref(q.pending_join_request_count, _::from_JV((v as _object)["pending_join_request_count"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatinvitelink? ) : chatinvitelink?
    return null if v==null
    var q = new chatinvitelink
    *q <- from_JV(v,type<chatinvitelink>)
    return q

struct chatadministratorrights
    //!  Represents the rights of an administrator in a chat.
    is_anonymous : bool	//! True, if the user's presence in the chat is hidden
    can_manage_chat : bool	//! True, if the administrator can access the chat event log, chat statistics, message statistics in channels, see channel members, see anonymous administrators in supergroups and ignore slow mode. Implied by any other administrator privilege
    can_delete_messages : bool	//! True, if the administrator can delete messages of other users
    can_manage_video_chats : bool	//! True, if the administrator can manage video chats
    can_restrict_members : bool	//! True, if the administrator can restrict, ban or unban chat members
    can_promote_members : bool	//! True, if the administrator can add new administrators with a subset of their own privileges or demote administrators that they have promoted, directly or indirectly (promoted by administrators that were appointed by the user)
    can_change_info : bool	//! True, if the user is allowed to change the chat title, photo and other settings
    can_invite_users : bool	//! True, if the user is allowed to invite new users to the chat
    [[optional]] can_post_messages : bool	//! Optional. True, if the administrator can post in the channel; channels only
    [[optional]] can_edit_messages : bool	//! Optional. True, if the administrator can edit messages of other users and can pin messages; channels only
    [[optional]] can_pin_messages : bool	//! Optional. True, if the user is allowed to pin messages; groups and supergroups only
    [[optional]] can_manage_topics : bool	//! Optional. True, if the user is allowed to create, rename, close, and reopen forum topics; supergroups only

def from_JV ( var v:JsonValue?; t:chatadministratorrights ) : chatadministratorrights
    var q : chatadministratorrights
    move_to_ref(q.is_anonymous, _::from_JV((v as _object)["is_anonymous"], type<bool>))
    move_to_ref(q.can_manage_chat, _::from_JV((v as _object)["can_manage_chat"], type<bool>))
    move_to_ref(q.can_delete_messages, _::from_JV((v as _object)["can_delete_messages"], type<bool>))
    move_to_ref(q.can_manage_video_chats, _::from_JV((v as _object)["can_manage_video_chats"], type<bool>))
    move_to_ref(q.can_restrict_members, _::from_JV((v as _object)["can_restrict_members"], type<bool>))
    move_to_ref(q.can_promote_members, _::from_JV((v as _object)["can_promote_members"], type<bool>))
    move_to_ref(q.can_change_info, _::from_JV((v as _object)["can_change_info"], type<bool>))
    move_to_ref(q.can_invite_users, _::from_JV((v as _object)["can_invite_users"], type<bool>))
    move_to_ref(q.can_post_messages, _::from_JV((v as _object)["can_post_messages"], type<bool>))
    move_to_ref(q.can_edit_messages, _::from_JV((v as _object)["can_edit_messages"], type<bool>))
    move_to_ref(q.can_pin_messages, _::from_JV((v as _object)["can_pin_messages"], type<bool>))
    move_to_ref(q.can_manage_topics, _::from_JV((v as _object)["can_manage_topics"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatadministratorrights? ) : chatadministratorrights?
    return null if v==null
    var q = new chatadministratorrights
    *q <- from_JV(v,type<chatadministratorrights>)
    return q

struct chatmember
    //!  This object contains information about one member of a chat. Currently, the following 6 types of chat members are supported:
    object_chatmember_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:chatmember ) : chatmember
    var q : chatmember
    return <- q

def from_JV ( var v:JsonValue?; t:chatmember? ) : chatmember?
    return null if v==null
    var q = new chatmember
    *q <- from_JV(v,type<chatmember>)
    return q

struct chatmemberowner
    //!  Represents a chat member that owns the chat and has all administrator privileges.
    status : string	//! The member's status in the chat, always “creator”
    user : user	//! Information about the user
    is_anonymous : bool	//! True, if the user's presence in the chat is hidden
    [[optional]] custom_title : string	//! Optional. Custom title for this user

def from_JV ( var v:JsonValue?; t:chatmemberowner ) : chatmemberowner
    var q : chatmemberowner
    move_to_ref(q.status, _::from_JV((v as _object)["status"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    move_to_ref(q.is_anonymous, _::from_JV((v as _object)["is_anonymous"], type<bool>))
    move_to_ref(q.custom_title, _::from_JV((v as _object)["custom_title"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmemberowner? ) : chatmemberowner?
    return null if v==null
    var q = new chatmemberowner
    *q <- from_JV(v,type<chatmemberowner>)
    return q

struct chatmemberadministrator
    //!  Represents a chat member that has some additional privileges.
    status : string	//! The member's status in the chat, always “administrator”
    user : user	//! Information about the user
    can_be_edited : bool	//! True, if the bot is allowed to edit administrator privileges of that user
    is_anonymous : bool	//! True, if the user's presence in the chat is hidden
    can_manage_chat : bool	//! True, if the administrator can access the chat event log, chat statistics, message statistics in channels, see channel members, see anonymous administrators in supergroups and ignore slow mode. Implied by any other administrator privilege
    can_delete_messages : bool	//! True, if the administrator can delete messages of other users
    can_manage_video_chats : bool	//! True, if the administrator can manage video chats
    can_restrict_members : bool	//! True, if the administrator can restrict, ban or unban chat members
    can_promote_members : bool	//! True, if the administrator can add new administrators with a subset of their own privileges or demote administrators that they have promoted, directly or indirectly (promoted by administrators that were appointed by the user)
    can_change_info : bool	//! True, if the user is allowed to change the chat title, photo and other settings
    can_invite_users : bool	//! True, if the user is allowed to invite new users to the chat
    [[optional]] can_post_messages : bool	//! Optional. True, if the administrator can post in the channel; channels only
    [[optional]] can_edit_messages : bool	//! Optional. True, if the administrator can edit messages of other users and can pin messages; channels only
    [[optional]] can_pin_messages : bool	//! Optional. True, if the user is allowed to pin messages; groups and supergroups only
    [[optional]] can_manage_topics : bool	//! Optional. True, if the user is allowed to create, rename, close, and reopen forum topics; supergroups only
    [[optional]] custom_title : string	//! Optional. Custom title for this user

def from_JV ( var v:JsonValue?; t:chatmemberadministrator ) : chatmemberadministrator
    var q : chatmemberadministrator
    move_to_ref(q.status, _::from_JV((v as _object)["status"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    move_to_ref(q.can_be_edited, _::from_JV((v as _object)["can_be_edited"], type<bool>))
    move_to_ref(q.is_anonymous, _::from_JV((v as _object)["is_anonymous"], type<bool>))
    move_to_ref(q.can_manage_chat, _::from_JV((v as _object)["can_manage_chat"], type<bool>))
    move_to_ref(q.can_delete_messages, _::from_JV((v as _object)["can_delete_messages"], type<bool>))
    move_to_ref(q.can_manage_video_chats, _::from_JV((v as _object)["can_manage_video_chats"], type<bool>))
    move_to_ref(q.can_restrict_members, _::from_JV((v as _object)["can_restrict_members"], type<bool>))
    move_to_ref(q.can_promote_members, _::from_JV((v as _object)["can_promote_members"], type<bool>))
    move_to_ref(q.can_change_info, _::from_JV((v as _object)["can_change_info"], type<bool>))
    move_to_ref(q.can_invite_users, _::from_JV((v as _object)["can_invite_users"], type<bool>))
    move_to_ref(q.can_post_messages, _::from_JV((v as _object)["can_post_messages"], type<bool>))
    move_to_ref(q.can_edit_messages, _::from_JV((v as _object)["can_edit_messages"], type<bool>))
    move_to_ref(q.can_pin_messages, _::from_JV((v as _object)["can_pin_messages"], type<bool>))
    move_to_ref(q.can_manage_topics, _::from_JV((v as _object)["can_manage_topics"], type<bool>))
    move_to_ref(q.custom_title, _::from_JV((v as _object)["custom_title"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmemberadministrator? ) : chatmemberadministrator?
    return null if v==null
    var q = new chatmemberadministrator
    *q <- from_JV(v,type<chatmemberadministrator>)
    return q

struct chatmembermember
    //!  Represents a chat member that has no additional privileges or restrictions.
    status : string	//! The member's status in the chat, always “member”
    user : user	//! Information about the user

def from_JV ( var v:JsonValue?; t:chatmembermember ) : chatmembermember
    var q : chatmembermember
    move_to_ref(q.status, _::from_JV((v as _object)["status"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmembermember? ) : chatmembermember?
    return null if v==null
    var q = new chatmembermember
    *q <- from_JV(v,type<chatmembermember>)
    return q

struct chatmemberrestricted
    //!  Represents a chat member that is under certain restrictions in the chat. Supergroups only.
    status : string	//! The member's status in the chat, always “restricted”
    user : user	//! Information about the user
    is_member : bool	//! True, if the user is a member of the chat at the moment of the request
    can_send_messages : bool	//! True, if the user is allowed to send text messages, contacts, invoices, locations and venues
    can_send_audios : bool	//! True, if the user is allowed to send audios
    can_send_documents : bool	//! True, if the user is allowed to send documents
    can_send_photos : bool	//! True, if the user is allowed to send photos
    can_send_videos : bool	//! True, if the user is allowed to send videos
    can_send_video_notes : bool	//! True, if the user is allowed to send video notes
    can_send_voice_notes : bool	//! True, if the user is allowed to send voice notes
    can_send_polls : bool	//! True, if the user is allowed to send polls
    can_send_other_messages : bool	//! True, if the user is allowed to send animations, games, stickers and use inline bots
    can_add_web_page_previews : bool	//! True, if the user is allowed to add web page previews to their messages
    can_change_info : bool	//! True, if the user is allowed to change the chat title, photo and other settings
    can_invite_users : bool	//! True, if the user is allowed to invite new users to the chat
    can_pin_messages : bool	//! True, if the user is allowed to pin messages
    can_manage_topics : bool	//! True, if the user is allowed to create forum topics
    until_date : int	//! Date when restrictions will be lifted for this user; unix time. If 0, then the user is restricted forever

def from_JV ( var v:JsonValue?; t:chatmemberrestricted ) : chatmemberrestricted
    var q : chatmemberrestricted
    move_to_ref(q.status, _::from_JV((v as _object)["status"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    move_to_ref(q.is_member, _::from_JV((v as _object)["is_member"], type<bool>))
    move_to_ref(q.can_send_messages, _::from_JV((v as _object)["can_send_messages"], type<bool>))
    move_to_ref(q.can_send_audios, _::from_JV((v as _object)["can_send_audios"], type<bool>))
    move_to_ref(q.can_send_documents, _::from_JV((v as _object)["can_send_documents"], type<bool>))
    move_to_ref(q.can_send_photos, _::from_JV((v as _object)["can_send_photos"], type<bool>))
    move_to_ref(q.can_send_videos, _::from_JV((v as _object)["can_send_videos"], type<bool>))
    move_to_ref(q.can_send_video_notes, _::from_JV((v as _object)["can_send_video_notes"], type<bool>))
    move_to_ref(q.can_send_voice_notes, _::from_JV((v as _object)["can_send_voice_notes"], type<bool>))
    move_to_ref(q.can_send_polls, _::from_JV((v as _object)["can_send_polls"], type<bool>))
    move_to_ref(q.can_send_other_messages, _::from_JV((v as _object)["can_send_other_messages"], type<bool>))
    move_to_ref(q.can_add_web_page_previews, _::from_JV((v as _object)["can_add_web_page_previews"], type<bool>))
    move_to_ref(q.can_change_info, _::from_JV((v as _object)["can_change_info"], type<bool>))
    move_to_ref(q.can_invite_users, _::from_JV((v as _object)["can_invite_users"], type<bool>))
    move_to_ref(q.can_pin_messages, _::from_JV((v as _object)["can_pin_messages"], type<bool>))
    move_to_ref(q.can_manage_topics, _::from_JV((v as _object)["can_manage_topics"], type<bool>))
    move_to_ref(q.until_date, _::from_JV((v as _object)["until_date"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmemberrestricted? ) : chatmemberrestricted?
    return null if v==null
    var q = new chatmemberrestricted
    *q <- from_JV(v,type<chatmemberrestricted>)
    return q

struct chatmemberleft
    //!  Represents a chat member that isn't currently a member of the chat, but may join it themselves.
    status : string	//! The member's status in the chat, always “left”
    user : user	//! Information about the user

def from_JV ( var v:JsonValue?; t:chatmemberleft ) : chatmemberleft
    var q : chatmemberleft
    move_to_ref(q.status, _::from_JV((v as _object)["status"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmemberleft? ) : chatmemberleft?
    return null if v==null
    var q = new chatmemberleft
    *q <- from_JV(v,type<chatmemberleft>)
    return q

struct chatmemberbanned
    //!  Represents a chat member that was banned in the chat and can't return to the chat or view chat messages.
    status : string	//! The member's status in the chat, always “kicked”
    user : user	//! Information about the user
    until_date : int	//! Date when restrictions will be lifted for this user; unix time. If 0, then the user is banned forever

def from_JV ( var v:JsonValue?; t:chatmemberbanned ) : chatmemberbanned
    var q : chatmemberbanned
    move_to_ref(q.status, _::from_JV((v as _object)["status"], type<string>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    move_to_ref(q.until_date, _::from_JV((v as _object)["until_date"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmemberbanned? ) : chatmemberbanned?
    return null if v==null
    var q = new chatmemberbanned
    *q <- from_JV(v,type<chatmemberbanned>)
    return q

struct chatmemberupdated
    //!  This object represents changes in the status of a chat member.
    chat : chat	//! Chat the user belongs to
    from : user	//! Performer of the action, which resulted in the change
    date : int	//! Date the change was done in Unix time
    old_chat_member : chatmember	//! Previous information about the chat member
    new_chat_member : chatmember	//! New information about the chat member
    [[optional]] invite_link : chatinvitelink?	//! Optional. Chat invite link, which was used by the user to join the chat; for joining by invite link events only.

def from_JV ( var v:JsonValue?; t:chatmemberupdated ) : chatmemberupdated
    var q : chatmemberupdated
    move_to_ref(q.chat, _::from_JV((v as _object)["chat"], type<chat>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.date, _::from_JV((v as _object)["date"], type<int>))
    move_to_ref(q.old_chat_member, _::from_JV((v as _object)["old_chat_member"], type<chatmember>))
    move_to_ref(q.new_chat_member, _::from_JV((v as _object)["new_chat_member"], type<chatmember>))
    move_to_ref(q.invite_link, _::from_JV((v as _object)["invite_link"], type<chatinvitelink?>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatmemberupdated? ) : chatmemberupdated?
    return null if v==null
    var q = new chatmemberupdated
    *q <- from_JV(v,type<chatmemberupdated>)
    return q

struct chatjoinrequest
    //!  Represents a join request sent to a chat.
    chat : chat	//! Chat to which the request was sent
    from : user	//! User that sent the join request
    user_chat_id : int	//! Identifier of a private chat with the user who sent the join request. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier. The bot can use this identifier for 24 hours to send messages until the join request is processed, assuming no other administrator contacted the user.
    date : int	//! Date the request was sent in Unix time
    [[optional]] bio : string	//! Optional. Bio of the user.
    [[optional]] invite_link : chatinvitelink?	//! Optional. Chat invite link that was used by the user to send the join request

def from_JV ( var v:JsonValue?; t:chatjoinrequest ) : chatjoinrequest
    var q : chatjoinrequest
    move_to_ref(q.chat, _::from_JV((v as _object)["chat"], type<chat>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.user_chat_id, _::from_JV((v as _object)["user_chat_id"], type<int>))
    move_to_ref(q.date, _::from_JV((v as _object)["date"], type<int>))
    move_to_ref(q.bio, _::from_JV((v as _object)["bio"], type<string>))
    move_to_ref(q.invite_link, _::from_JV((v as _object)["invite_link"], type<chatinvitelink?>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatjoinrequest? ) : chatjoinrequest?
    return null if v==null
    var q = new chatjoinrequest
    *q <- from_JV(v,type<chatjoinrequest>)
    return q

struct chatpermissions
    //!  Describes actions that a non-administrator user is allowed to take in a chat.
    [[optional]] can_send_messages : bool	//! Optional. True, if the user is allowed to send text messages, contacts, invoices, locations and venues
    [[optional]] can_send_audios : bool	//! Optional. True, if the user is allowed to send audios
    [[optional]] can_send_documents : bool	//! Optional. True, if the user is allowed to send documents
    [[optional]] can_send_photos : bool	//! Optional. True, if the user is allowed to send photos
    [[optional]] can_send_videos : bool	//! Optional. True, if the user is allowed to send videos
    [[optional]] can_send_video_notes : bool	//! Optional. True, if the user is allowed to send video notes
    [[optional]] can_send_voice_notes : bool	//! Optional. True, if the user is allowed to send voice notes
    [[optional]] can_send_polls : bool	//! Optional. True, if the user is allowed to send polls
    [[optional]] can_send_other_messages : bool	//! Optional. True, if the user is allowed to send animations, games, stickers and use inline bots
    [[optional]] can_add_web_page_previews : bool	//! Optional. True, if the user is allowed to add web page previews to their messages
    [[optional]] can_change_info : bool	//! Optional. True, if the user is allowed to change the chat title, photo and other settings. Ignored in public supergroups
    [[optional]] can_invite_users : bool	//! Optional. True, if the user is allowed to invite new users to the chat
    [[optional]] can_pin_messages : bool	//! Optional. True, if the user is allowed to pin messages. Ignored in public supergroups
    [[optional]] can_manage_topics : bool	//! Optional. True, if the user is allowed to create forum topics. If omitted defaults to the value of can_pin_messages

def from_JV ( var v:JsonValue?; t:chatpermissions ) : chatpermissions
    var q : chatpermissions
    move_to_ref(q.can_send_messages, _::from_JV((v as _object)["can_send_messages"], type<bool>))
    move_to_ref(q.can_send_audios, _::from_JV((v as _object)["can_send_audios"], type<bool>))
    move_to_ref(q.can_send_documents, _::from_JV((v as _object)["can_send_documents"], type<bool>))
    move_to_ref(q.can_send_photos, _::from_JV((v as _object)["can_send_photos"], type<bool>))
    move_to_ref(q.can_send_videos, _::from_JV((v as _object)["can_send_videos"], type<bool>))
    move_to_ref(q.can_send_video_notes, _::from_JV((v as _object)["can_send_video_notes"], type<bool>))
    move_to_ref(q.can_send_voice_notes, _::from_JV((v as _object)["can_send_voice_notes"], type<bool>))
    move_to_ref(q.can_send_polls, _::from_JV((v as _object)["can_send_polls"], type<bool>))
    move_to_ref(q.can_send_other_messages, _::from_JV((v as _object)["can_send_other_messages"], type<bool>))
    move_to_ref(q.can_add_web_page_previews, _::from_JV((v as _object)["can_add_web_page_previews"], type<bool>))
    move_to_ref(q.can_change_info, _::from_JV((v as _object)["can_change_info"], type<bool>))
    move_to_ref(q.can_invite_users, _::from_JV((v as _object)["can_invite_users"], type<bool>))
    move_to_ref(q.can_pin_messages, _::from_JV((v as _object)["can_pin_messages"], type<bool>))
    move_to_ref(q.can_manage_topics, _::from_JV((v as _object)["can_manage_topics"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatpermissions? ) : chatpermissions?
    return null if v==null
    var q = new chatpermissions
    *q <- from_JV(v,type<chatpermissions>)
    return q

struct chatlocation
    //!  Represents a location to which a chat is connected.
    location : location	//! The location to which the supergroup is connected. Can't be a live location.
    address : string	//! Location address; 1-64 characters, as defined by the chat owner

def from_JV ( var v:JsonValue?; t:chatlocation ) : chatlocation
    var q : chatlocation
    move_to_ref(q.location, _::from_JV((v as _object)["location"], type<location>))
    move_to_ref(q.address, _::from_JV((v as _object)["address"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:chatlocation? ) : chatlocation?
    return null if v==null
    var q = new chatlocation
    *q <- from_JV(v,type<chatlocation>)
    return q

struct forumtopic
    //!  This object represents a forum topic.
    message_thread_id : int	//! Unique identifier of the forum topic
    name : string	//! Name of the topic
    icon_color : int	//! Color of the topic icon in RGB format
    [[optional]] icon_custom_emoji_id : string	//! Optional. Unique identifier of the custom emoji shown as the topic icon

def from_JV ( var v:JsonValue?; t:forumtopic ) : forumtopic
    var q : forumtopic
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.icon_color, _::from_JV((v as _object)["icon_color"], type<int>))
    move_to_ref(q.icon_custom_emoji_id, _::from_JV((v as _object)["icon_custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:forumtopic? ) : forumtopic?
    return null if v==null
    var q = new forumtopic
    *q <- from_JV(v,type<forumtopic>)
    return q

struct botcommand
    //!  This object represents a bot command.
    command : string	//! Text of the command; 1-32 characters. Can contain only lowercase English letters, digits and underscores.
    description : string	//! Description of the command; 1-256 characters.

def from_JV ( var v:JsonValue?; t:botcommand ) : botcommand
    var q : botcommand
    move_to_ref(q.command, _::from_JV((v as _object)["command"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommand? ) : botcommand?
    return null if v==null
    var q = new botcommand
    *q <- from_JV(v,type<botcommand>)
    return q

struct botcommandscope
    //!  This object represents the scope to which bot commands are applied. Currently, the following 7 scopes are supported:
    object_botcommandscope_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:botcommandscope ) : botcommandscope
    var q : botcommandscope
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscope? ) : botcommandscope?
    return null if v==null
    var q = new botcommandscope
    *q <- from_JV(v,type<botcommandscope>)
    return q

struct determining_list_of_commands
    //!  The following algorithm is used to determine the list of commands for a particular user viewing the bot menu. The first list of commands which is set is returned: Commands in the chat with the bot Commands in group and supergroup chats
    object_determining_list_of_commands_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:determining_list_of_commands ) : determining_list_of_commands
    var q : determining_list_of_commands
    return <- q

def from_JV ( var v:JsonValue?; t:determining_list_of_commands? ) : determining_list_of_commands?
    return null if v==null
    var q = new determining_list_of_commands
    *q <- from_JV(v,type<determining_list_of_commands>)
    return q

struct botcommandscopedefault
    //!  Represents the default scope of bot commands. Default commands are used if no commands with a narrower scope are specified for the user.
    _type : string	//! Scope type, must be default

def from_JV ( var v:JsonValue?; t:botcommandscopedefault ) : botcommandscopedefault
    var q : botcommandscopedefault
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopedefault? ) : botcommandscopedefault?
    return null if v==null
    var q = new botcommandscopedefault
    *q <- from_JV(v,type<botcommandscopedefault>)
    return q

struct botcommandscopeallprivatechats
    //!  Represents the scope of bot commands, covering all private chats.
    _type : string	//! Scope type, must be all_private_chats

def from_JV ( var v:JsonValue?; t:botcommandscopeallprivatechats ) : botcommandscopeallprivatechats
    var q : botcommandscopeallprivatechats
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopeallprivatechats? ) : botcommandscopeallprivatechats?
    return null if v==null
    var q = new botcommandscopeallprivatechats
    *q <- from_JV(v,type<botcommandscopeallprivatechats>)
    return q

struct botcommandscopeallgroupchats
    //!  Represents the scope of bot commands, covering all group and supergroup chats.
    _type : string	//! Scope type, must be all_group_chats

def from_JV ( var v:JsonValue?; t:botcommandscopeallgroupchats ) : botcommandscopeallgroupchats
    var q : botcommandscopeallgroupchats
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopeallgroupchats? ) : botcommandscopeallgroupchats?
    return null if v==null
    var q = new botcommandscopeallgroupchats
    *q <- from_JV(v,type<botcommandscopeallgroupchats>)
    return q

struct botcommandscopeallchatadministrators
    //!  Represents the scope of bot commands, covering all group and supergroup chat administrators.
    _type : string	//! Scope type, must be all_chat_administrators

def from_JV ( var v:JsonValue?; t:botcommandscopeallchatadministrators ) : botcommandscopeallchatadministrators
    var q : botcommandscopeallchatadministrators
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopeallchatadministrators? ) : botcommandscopeallchatadministrators?
    return null if v==null
    var q = new botcommandscopeallchatadministrators
    *q <- from_JV(v,type<botcommandscopeallchatadministrators>)
    return q

struct botcommandscopechat
    //!  Represents the scope of bot commands, covering a specific chat.
    _type : string	//! Scope type, must be chat
    chat_id : string	//! Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:botcommandscopechat ) : botcommandscopechat
    var q : botcommandscopechat
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopechat? ) : botcommandscopechat?
    return null if v==null
    var q = new botcommandscopechat
    *q <- from_JV(v,type<botcommandscopechat>)
    return q

struct botcommandscopechatadministrators
    //!  Represents the scope of bot commands, covering all administrators of a specific group or supergroup chat.
    _type : string	//! Scope type, must be chat_administrators
    chat_id : string	//! Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:botcommandscopechatadministrators ) : botcommandscopechatadministrators
    var q : botcommandscopechatadministrators
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopechatadministrators? ) : botcommandscopechatadministrators?
    return null if v==null
    var q = new botcommandscopechatadministrators
    *q <- from_JV(v,type<botcommandscopechatadministrators>)
    return q

struct botcommandscopechatmember
    //!  Represents the scope of bot commands, covering a specific member of a group or supergroup chat.
    _type : string	//! Scope type, must be chat_member
    chat_id : string	//! Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    user_id : int	//! Unique identifier of the target user

def from_JV ( var v:JsonValue?; t:botcommandscopechatmember ) : botcommandscopechatmember
    var q : botcommandscopechatmember
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:botcommandscopechatmember? ) : botcommandscopechatmember?
    return null if v==null
    var q = new botcommandscopechatmember
    *q <- from_JV(v,type<botcommandscopechatmember>)
    return q

struct botdescription
    //!  This object represents the bot's description.
    description : string	//! The bot's description

def from_JV ( var v:JsonValue?; t:botdescription ) : botdescription
    var q : botdescription
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botdescription? ) : botdescription?
    return null if v==null
    var q = new botdescription
    *q <- from_JV(v,type<botdescription>)
    return q

struct botshortdescription
    //!  This object represents the bot's short description.
    short_description : string	//! The bot's short description

def from_JV ( var v:JsonValue?; t:botshortdescription ) : botshortdescription
    var q : botshortdescription
    move_to_ref(q.short_description, _::from_JV((v as _object)["short_description"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:botshortdescription? ) : botshortdescription?
    return null if v==null
    var q = new botshortdescription
    *q <- from_JV(v,type<botshortdescription>)
    return q

struct menubutton
    //!  This object describes the bot's menu button in a private chat. It should be one of If a menu button other than MenuButtonDefault is set for a private chat, then it is applied in the chat. Otherwise the default menu button is applied. By default, the menu button opens the list of bot commands.
    object_menubutton_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:menubutton ) : menubutton
    var q : menubutton
    return <- q

def from_JV ( var v:JsonValue?; t:menubutton? ) : menubutton?
    return null if v==null
    var q = new menubutton
    *q <- from_JV(v,type<menubutton>)
    return q

struct menubuttoncommands
    //!  Represents a menu button, which opens the bot's list of commands.
    _type : string	//! Type of the button, must be commands

def from_JV ( var v:JsonValue?; t:menubuttoncommands ) : menubuttoncommands
    var q : menubuttoncommands
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:menubuttoncommands? ) : menubuttoncommands?
    return null if v==null
    var q = new menubuttoncommands
    *q <- from_JV(v,type<menubuttoncommands>)
    return q

struct menubuttonwebapp
    //!  Represents a menu button, which launches a Web App.
    _type : string	//! Type of the button, must be web_app
    text : string	//! Text on the button
    web_app : webappinfo	//! Description of the Web App that will be launched when the user presses the button. The Web App will be able to send an arbitrary message on behalf of the user using the method answerWebAppQuery.

def from_JV ( var v:JsonValue?; t:menubuttonwebapp ) : menubuttonwebapp
    var q : menubuttonwebapp
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.web_app, _::from_JV((v as _object)["web_app"], type<webappinfo>))
    return <- q

def from_JV ( var v:JsonValue?; t:menubuttonwebapp? ) : menubuttonwebapp?
    return null if v==null
    var q = new menubuttonwebapp
    *q <- from_JV(v,type<menubuttonwebapp>)
    return q

struct menubuttondefault
    //!  Describes that no specific value for the menu button was set.
    _type : string	//! Type of the button, must be default

def from_JV ( var v:JsonValue?; t:menubuttondefault ) : menubuttondefault
    var q : menubuttondefault
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:menubuttondefault? ) : menubuttondefault?
    return null if v==null
    var q = new menubuttondefault
    *q <- from_JV(v,type<menubuttondefault>)
    return q

struct responseparameters
    //!  Describes why a request was unsuccessful.
    [[optional]] migrate_to_chat_id : int	//! Optional. The group has been migrated to a supergroup with the specified identifier. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] retry_after : int	//! Optional. In case of exceeding flood control, the number of seconds left to wait before the request can be repeated

def from_JV ( var v:JsonValue?; t:responseparameters ) : responseparameters
    var q : responseparameters
    move_to_ref(q.migrate_to_chat_id, _::from_JV((v as _object)["migrate_to_chat_id"], type<int>))
    move_to_ref(q.retry_after, _::from_JV((v as _object)["retry_after"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:responseparameters? ) : responseparameters?
    return null if v==null
    var q = new responseparameters
    *q <- from_JV(v,type<responseparameters>)
    return q

struct inputmedia
    //!  This object represents the content of a media message to be sent. It should be one of
    object_inputmedia_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:inputmedia ) : inputmedia
    var q : inputmedia
    return <- q

def from_JV ( var v:JsonValue?; t:inputmedia? ) : inputmedia?
    return null if v==null
    var q = new inputmedia
    *q <- from_JV(v,type<inputmedia>)
    return q

struct inputmediaphoto
    //!  Represents a photo to be sent.
    _type : string	//! Type of the result, must be photo
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Caption of the photo to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool	//! Optional. Pass True if the photo needs to be covered with a spoiler animation

def from_JV ( var v:JsonValue?; t:inputmediaphoto ) : inputmediaphoto
    var q : inputmediaphoto
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.has_spoiler, _::from_JV((v as _object)["has_spoiler"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputmediaphoto? ) : inputmediaphoto?
    return null if v==null
    var q = new inputmediaphoto
    *q <- from_JV(v,type<inputmediaphoto>)
    return q

struct inputmediavideo
    //!  Represents a video to be sent.
    _type : string	//! Type of the result, must be video
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Caption of the video to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] width : int	//! Optional. Video width
    [[optional]] height : int	//! Optional. Video height
    [[optional]] duration : int	//! Optional. Video duration in seconds
    [[optional]] supports_streaming : bool	//! Optional. Pass True if the uploaded video is suitable for streaming
    [[optional]] has_spoiler : bool	//! Optional. Pass True if the video needs to be covered with a spoiler animation

def from_JV ( var v:JsonValue?; t:inputmediavideo ) : inputmediavideo
    var q : inputmediavideo
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<string>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.supports_streaming, _::from_JV((v as _object)["supports_streaming"], type<bool>))
    move_to_ref(q.has_spoiler, _::from_JV((v as _object)["has_spoiler"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputmediavideo? ) : inputmediavideo?
    return null if v==null
    var q = new inputmediavideo
    *q <- from_JV(v,type<inputmediavideo>)
    return q

struct inputmediaanimation
    //!  Represents an animation file (GIF or H.264/MPEG-4 AVC video without sound) to be sent.
    _type : string	//! Type of the result, must be animation
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Caption of the animation to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the animation caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] width : int	//! Optional. Animation width
    [[optional]] height : int	//! Optional. Animation height
    [[optional]] duration : int	//! Optional. Animation duration in seconds
    [[optional]] has_spoiler : bool	//! Optional. Pass True if the animation needs to be covered with a spoiler animation

def from_JV ( var v:JsonValue?; t:inputmediaanimation ) : inputmediaanimation
    var q : inputmediaanimation
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<string>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.has_spoiler, _::from_JV((v as _object)["has_spoiler"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputmediaanimation? ) : inputmediaanimation?
    return null if v==null
    var q = new inputmediaanimation
    *q <- from_JV(v,type<inputmediaanimation>)
    return q

struct inputmediaaudio
    //!  Represents an audio file to be treated as music to be sent.
    _type : string	//! Type of the result, must be audio
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Caption of the audio to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] duration : int	//! Optional. Duration of the audio in seconds
    [[optional]] performer : string	//! Optional. Performer of the audio
    [[optional]] title : string	//! Optional. Title of the audio

def from_JV ( var v:JsonValue?; t:inputmediaaudio ) : inputmediaaudio
    var q : inputmediaaudio
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<string>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.performer, _::from_JV((v as _object)["performer"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputmediaaudio? ) : inputmediaaudio?
    return null if v==null
    var q = new inputmediaaudio
    *q <- from_JV(v,type<inputmediaaudio>)
    return q

struct inputmediadocument
    //!  Represents a general file to be sent.
    _type : string	//! Type of the result, must be document
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Caption of the document to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] disable_content_type_detection : bool	//! Optional. Disables automatic server-side content type detection for files uploaded using multipart/form-data. Always True, if the document is sent as part of an album.

def from_JV ( var v:JsonValue?; t:inputmediadocument ) : inputmediadocument
    var q : inputmediadocument
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<string>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.disable_content_type_detection, _::from_JV((v as _object)["disable_content_type_detection"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputmediadocument? ) : inputmediadocument?
    return null if v==null
    var q = new inputmediadocument
    *q <- from_JV(v,type<inputmediadocument>)
    return q

struct inputfile
    //!  This object represents the contents of a file to be uploaded. Must be posted using multipart/form-data in the usual way that files are uploaded via the browser.
    object_inputfile_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:inputfile ) : inputfile
    var q : inputfile
    return <- q

def from_JV ( var v:JsonValue?; t:inputfile? ) : inputfile?
    return null if v==null
    var q = new inputfile
    *q <- from_JV(v,type<inputfile>)
    return q

struct sending_files
    //!  There are three ways to send files (photos, stickers, audio, media, etc.): Sending by file_id Sending by URL
    object_sending_files_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:sending_files ) : sending_files
    var q : sending_files
    return <- q

def from_JV ( var v:JsonValue?; t:sending_files? ) : sending_files?
    return null if v==null
    var q = new sending_files
    *q <- from_JV(v,type<sending_files>)
    return q

struct inline_mode_objects
    //!  Objects and methods used in the inline mode are described in the Inline mode section.
    object_inline_mode_objects_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:inline_mode_objects ) : inline_mode_objects
    var q : inline_mode_objects
    return <- q

def from_JV ( var v:JsonValue?; t:inline_mode_objects? ) : inline_mode_objects?
    return null if v==null
    var q = new inline_mode_objects
    *q <- from_JV(v,type<inline_mode_objects>)
    return q

struct getme
    //!  A simple method for testing your bot's authentication token. Requires no parameters. Returns basic information about the bot in form of a User object.
    object_getme_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:getme ) : getme
    var q : getme
    return <- q

def from_JV ( var v:JsonValue?; t:getme? ) : getme?
    return null if v==null
    var q = new getme
    *q <- from_JV(v,type<getme>)
    return q

struct logout
    //!  Use this method to log out from the cloud Bot API server before launching the bot locally. You must log out the bot before running it locally, otherwise there is no guarantee that the bot will receive updates. After a successful call, you can immediately log in on a local server, but will not be able to log in back to the cloud Bot API server for 10 minutes. Returns True on success. Requires no parameters.
    object_logout_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:logout ) : logout
    var q : logout
    return <- q

def from_JV ( var v:JsonValue?; t:logout? ) : logout?
    return null if v==null
    var q = new logout
    *q <- from_JV(v,type<logout>)
    return q

struct close
    //!  Use this method to close the bot instance before moving it from one local server to another. You need to delete the webhook before calling this method to ensure that the bot isn't launched again after server restart. The method will return error 429 in the first 10 minutes after the bot is launched. Returns True on success. Requires no parameters.
    object_close_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:close ) : close
    var q : close
    return <- q

def from_JV ( var v:JsonValue?; t:close? ) : close?
    return null if v==null
    var q = new close
    *q <- from_JV(v,type<close>)
    return q

struct sendmessage
    //!  Use this method to send text messages. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    text : string	//! Required. Text of the message to be sent, 1-4096 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the message text. See formatting options for more details.
    [[optional]] entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in message text, which can be specified instead of parse_mode
    [[optional]] disable_web_page_preview : bool	//! Optional. Disables link previews for links in this message
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendmessage ) : sendmessage
    var q : sendmessage
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.entities, _::from_JV((v as _object)["entities"], type<array<messageentity>>))
    move_to_ref(q.disable_web_page_preview, _::from_JV((v as _object)["disable_web_page_preview"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendmessage? ) : sendmessage?
    return null if v==null
    var q = new sendmessage
    *q <- from_JV(v,type<sendmessage>)
    return q

struct formatting_options
    //!  The Bot API supports basic formatting for messages. You can use bold, italic, underlined, strikethrough, and spoiler text, as well as inline links and pre-formatted code in your bots' messages. Telegram clients will render them accordingly. You can specify text entities directly, or use markdown-style or HTML-style formatting. Note that Telegram clients will display an alert to the user before opening an inline link ('Open this link?' together with the full URL). Message entities can be nested, providing following restrictions are met: - If two entities have common characters, then one of them is fully contained inside another. - bold, italic, underline, strikethrough, and spoiler entities can contain and can be part of any other entities, except pre and code. - All other entities can't contain each other. Links tg://user?id=<user_id> can be used to mention a user by their ID without using a username. Please note: To use this mode, pass MarkdownV2 in the parse_mode field. Use the following syntax in your message: Please note: To use this mode, pass HTML in the parse_mode field. The following tags are currently supported: Please note: This is a legacy mode, retained for backward compatibility. To use this mode, pass Markdown in the parse_mode field. Use the following syntax in your message: Please note:
    object_formatting_options_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:formatting_options ) : formatting_options
    var q : formatting_options
    return <- q

def from_JV ( var v:JsonValue?; t:formatting_options? ) : formatting_options?
    return null if v==null
    var q = new formatting_options
    *q <- from_JV(v,type<formatting_options>)
    return q

struct forwardmessage
    //!  Use this method to forward messages of any kind. Service messages can't be forwarded. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    from_chat_id : string	//! Required. Unique identifier for the chat where the original message was sent (or channel username in the format @channelusername)
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the forwarded message from forwarding and saving
    message_id : int	//! Required. Message identifier in the chat specified in from_chat_id

def from_JV ( var v:JsonValue?; t:forwardmessage ) : forwardmessage
    var q : forwardmessage
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.from_chat_id, _::from_JV((v as _object)["from_chat_id"], type<string>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:forwardmessage? ) : forwardmessage?
    return null if v==null
    var q = new forwardmessage
    *q <- from_JV(v,type<forwardmessage>)
    return q

struct copymessage
    //!  Use this method to copy messages of any kind. Service messages and invoice messages can't be copied. A quiz poll can be copied only if the value of the field correct_option_id is known to the bot. The method is analogous to the method forwardMessage, but the copied message doesn't have a link to the original message. Returns the MessageId of the sent message on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    from_chat_id : string	//! Required. Unique identifier for the chat where the original message was sent (or channel username in the format @channelusername)
    message_id : int	//! Required. Message identifier in the chat specified in from_chat_id
    [[optional]] caption : string	//! Optional. New caption for media, 0-1024 characters after entities parsing. If not specified, the original caption is kept
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the new caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the new caption, which can be specified instead of parse_mode
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:copymessage ) : copymessage
    var q : copymessage
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.from_chat_id, _::from_JV((v as _object)["from_chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:copymessage? ) : copymessage?
    return null if v==null
    var q = new copymessage
    *q <- from_JV(v,type<copymessage>)
    return q

struct sendphoto
    //!  Use this method to send photos. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    photo : inputfile	//! Required. Photo to send. Pass a file_id as String to send a photo that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a photo from the Internet, or upload a new photo using multipart/form-data. The photo must be at most 10 MB in size. The photo's width and height must not exceed 10000 in total. Width and height ratio must be at most 20. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Photo caption (may also be used when resending photos by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool	//! Optional. Pass True if the photo needs to be covered with a spoiler animation
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendphoto ) : sendphoto
    var q : sendphoto
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.photo, _::from_JV((v as _object)["photo"], type<inputfile>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.has_spoiler, _::from_JV((v as _object)["has_spoiler"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendphoto? ) : sendphoto?
    return null if v==null
    var q = new sendphoto
    *q <- from_JV(v,type<sendphoto>)
    return q

struct sendaudio
    //!  Use this method to send audio files, if you want Telegram clients to display them in the music player. Your audio must be in the .MP3 or .M4A format. On success, the sent Message is returned. Bots can currently send audio files of up to 50 MB in size, this limit may be changed in the future. For sending voice messages, use the sendVoice method instead.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    audio : inputfile	//! Required. Audio file to send. Pass a file_id as String to send an audio file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get an audio file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Audio caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] duration : int	//! Optional. Duration of the audio in seconds
    [[optional]] performer : string	//! Optional. Performer
    [[optional]] title : string	//! Optional. Track name
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendaudio ) : sendaudio
    var q : sendaudio
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.audio, _::from_JV((v as _object)["audio"], type<inputfile>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.performer, _::from_JV((v as _object)["performer"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendaudio? ) : sendaudio?
    return null if v==null
    var q = new sendaudio
    *q <- from_JV(v,type<sendaudio>)
    return q

struct senddocument
    //!  Use this method to send general files. On success, the sent Message is returned. Bots can currently send files of any type of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    document : inputfile	//! Required. File to send. Pass a file_id as String to send a file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Document caption (may also be used when resending documents by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] disable_content_type_detection : bool	//! Optional. Disables automatic server-side content type detection for files uploaded using multipart/form-data
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:senddocument ) : senddocument
    var q : senddocument
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.document, _::from_JV((v as _object)["document"], type<inputfile>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.disable_content_type_detection, _::from_JV((v as _object)["disable_content_type_detection"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:senddocument? ) : senddocument?
    return null if v==null
    var q = new senddocument
    *q <- from_JV(v,type<senddocument>)
    return q

struct sendvideo
    //!  Use this method to send video files, Telegram clients support MPEG4 videos (other formats may be sent as Document). On success, the sent Message is returned. Bots can currently send video files of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    video : inputfile	//! Required. Video to send. Pass a file_id as String to send a video that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a video from the Internet, or upload a new video using multipart/form-data. More information on Sending Files »
    [[optional]] duration : int	//! Optional. Duration of sent video in seconds
    [[optional]] width : int	//! Optional. Video width
    [[optional]] height : int	//! Optional. Video height
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Video caption (may also be used when resending videos by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool	//! Optional. Pass True if the video needs to be covered with a spoiler animation
    [[optional]] supports_streaming : bool	//! Optional. Pass True if the uploaded video is suitable for streaming
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendvideo ) : sendvideo
    var q : sendvideo
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.video, _::from_JV((v as _object)["video"], type<inputfile>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.has_spoiler, _::from_JV((v as _object)["has_spoiler"], type<bool>))
    move_to_ref(q.supports_streaming, _::from_JV((v as _object)["supports_streaming"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendvideo? ) : sendvideo?
    return null if v==null
    var q = new sendvideo
    *q <- from_JV(v,type<sendvideo>)
    return q

struct sendanimation
    //!  Use this method to send animation files (GIF or H.264/MPEG-4 AVC video without sound). On success, the sent Message is returned. Bots can currently send animation files of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    animation : inputfile	//! Required. Animation to send. Pass a file_id as String to send an animation that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get an animation from the Internet, or upload a new animation using multipart/form-data. More information on Sending Files »
    [[optional]] duration : int	//! Optional. Duration of sent animation in seconds
    [[optional]] width : int	//! Optional. Animation width
    [[optional]] height : int	//! Optional. Animation height
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Animation caption (may also be used when resending animation by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the animation caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool	//! Optional. Pass True if the animation needs to be covered with a spoiler animation
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendanimation ) : sendanimation
    var q : sendanimation
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.animation, _::from_JV((v as _object)["animation"], type<inputfile>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.has_spoiler, _::from_JV((v as _object)["has_spoiler"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendanimation? ) : sendanimation?
    return null if v==null
    var q = new sendanimation
    *q <- from_JV(v,type<sendanimation>)
    return q

struct sendvoice
    //!  Use this method to send audio files, if you want Telegram clients to display the file as a playable voice message. For this to work, your audio must be in an .OGG file encoded with OPUS (other formats may be sent as Audio or Document). On success, the sent Message is returned. Bots can currently send voice messages of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    voice : inputfile	//! Required. Audio file to send. Pass a file_id as String to send a file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files »
    [[optional]] caption : string	//! Optional. Voice message caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] duration : int	//! Optional. Duration of the voice message in seconds
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendvoice ) : sendvoice
    var q : sendvoice
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.voice, _::from_JV((v as _object)["voice"], type<inputfile>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendvoice? ) : sendvoice?
    return null if v==null
    var q = new sendvoice
    *q <- from_JV(v,type<sendvoice>)
    return q

struct sendvideonote
    //!  As of v.4.0, Telegram clients support rounded square MPEG4 videos of up to 1 minute long. Use this method to send video messages. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    video_note : inputfile	//! Required. Video note to send. Pass a file_id as String to send a video note that exists on the Telegram servers (recommended) or upload a new video using multipart/form-data. More information on Sending Files ». Sending video notes by a URL is currently unsupported
    [[optional]] duration : int	//! Optional. Duration of sent video in seconds
    [[optional]] length : int	//! Optional. Video width and height, i.e. diameter of the video message
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendvideonote ) : sendvideonote
    var q : sendvideonote
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.video_note, _::from_JV((v as _object)["video_note"], type<inputfile>))
    move_to_ref(q.duration, _::from_JV((v as _object)["duration"], type<int>))
    move_to_ref(q.length, _::from_JV((v as _object)["length"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendvideonote? ) : sendvideonote?
    return null if v==null
    var q = new sendvideonote
    *q <- from_JV(v,type<sendvideonote>)
    return q

struct sendmediagroup
    //!  Use this method to send a group of photos, videos, documents or audios as an album. Documents and audio files can be only grouped in an album with messages of the same type. On success, an array of Messages that were sent is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    media : array<inputmediaaudio>	//! Required. A JSON-serialized array describing messages to be sent, must include 2-10 items
    [[optional]] disable_notification : bool	//! Optional. Sends messages silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent messages from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the messages are a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found

def from_JV ( var v:JsonValue?; t:sendmediagroup ) : sendmediagroup
    var q : sendmediagroup
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<array<inputmediaaudio>>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendmediagroup? ) : sendmediagroup?
    return null if v==null
    var q = new sendmediagroup
    *q <- from_JV(v,type<sendmediagroup>)
    return q

struct sendlocation
    //!  Use this method to send point on the map. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    latitude : float	//! Required. Latitude of the location
    longitude : float	//! Required. Longitude of the location
    [[optional]] horizontal_accuracy : float	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int	//! Optional. Period in seconds for which the location will be updated (see Live Locations, should be between 60 and 86400.
    [[optional]] heading : int	//! Optional. For live locations, a direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int	//! Optional. For live locations, a maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendlocation ) : sendlocation
    var q : sendlocation
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.horizontal_accuracy, _::from_JV((v as _object)["horizontal_accuracy"], type<float>))
    move_to_ref(q.live_period, _::from_JV((v as _object)["live_period"], type<int>))
    move_to_ref(q.heading, _::from_JV((v as _object)["heading"], type<int>))
    move_to_ref(q.proximity_alert_radius, _::from_JV((v as _object)["proximity_alert_radius"], type<int>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendlocation? ) : sendlocation?
    return null if v==null
    var q = new sendlocation
    *q <- from_JV(v,type<sendlocation>)
    return q

struct sendvenue
    //!  Use this method to send information about a venue. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    latitude : float	//! Required. Latitude of the venue
    longitude : float	//! Required. Longitude of the venue
    title : string	//! Required. Name of the venue
    address : string	//! Required. Address of the venue
    [[optional]] foursquare_id : string	//! Optional. Foursquare identifier of the venue
    [[optional]] foursquare_type : string	//! Optional. Foursquare type of the venue, if known. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string	//! Optional. Google Places type of the venue. (See supported types.)
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendvenue ) : sendvenue
    var q : sendvenue
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.address, _::from_JV((v as _object)["address"], type<string>))
    move_to_ref(q.foursquare_id, _::from_JV((v as _object)["foursquare_id"], type<string>))
    move_to_ref(q.foursquare_type, _::from_JV((v as _object)["foursquare_type"], type<string>))
    move_to_ref(q.google_place_id, _::from_JV((v as _object)["google_place_id"], type<string>))
    move_to_ref(q.google_place_type, _::from_JV((v as _object)["google_place_type"], type<string>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendvenue? ) : sendvenue?
    return null if v==null
    var q = new sendvenue
    *q <- from_JV(v,type<sendvenue>)
    return q

struct sendcontact
    //!  Use this method to send phone contacts. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    phone_number : string	//! Required. Contact's phone number
    first_name : string	//! Required. Contact's first name
    [[optional]] last_name : string	//! Optional. Contact's last name
    [[optional]] vcard : string	//! Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendcontact ) : sendcontact
    var q : sendcontact
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.phone_number, _::from_JV((v as _object)["phone_number"], type<string>))
    move_to_ref(q.first_name, _::from_JV((v as _object)["first_name"], type<string>))
    move_to_ref(q.last_name, _::from_JV((v as _object)["last_name"], type<string>))
    move_to_ref(q.vcard, _::from_JV((v as _object)["vcard"], type<string>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendcontact? ) : sendcontact?
    return null if v==null
    var q = new sendcontact
    *q <- from_JV(v,type<sendcontact>)
    return q

struct sendpoll
    //!  Use this method to send a native poll. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    question : string	//! Required. Poll question, 1-300 characters
    _options : array<string>	//! Required. A JSON-serialized list of answer options, 2-10 strings 1-100 characters each
    [[optional]] is_anonymous : bool	//! Optional. True, if the poll needs to be anonymous, defaults to True
    [[optional]] _type : string	//! Optional. Poll type, “quiz” or “regular”, defaults to “regular”
    [[optional]] allows_multiple_answers : bool	//! Optional. True, if the poll allows multiple answers, ignored for polls in quiz mode, defaults to False
    [[optional]] correct_option_id : int	//! Optional. 0-based identifier of the correct answer option, required for polls in quiz mode
    [[optional]] explanation : string	//! Optional. Text that is shown when a user chooses an incorrect answer or taps on the lamp icon in a quiz-style poll, 0-200 characters with at most 2 line feeds after entities parsing
    [[optional]] explanation_parse_mode : string	//! Optional. Mode for parsing entities in the explanation. See formatting options for more details.
    [[optional]] explanation_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the poll explanation, which can be specified instead of parse_mode
    [[optional]] open_period : int	//! Optional. Amount of time in seconds the poll will be active after creation, 5-600. Can't be used together with close_date.
    [[optional]] close_date : int	//! Optional. Point in time (Unix timestamp) when the poll will be automatically closed. Must be at least 5 and no more than 600 seconds in the future. Can't be used together with open_period.
    [[optional]] is_closed : bool	//! Optional. Pass True if the poll needs to be immediately closed. This can be useful for poll preview.
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendpoll ) : sendpoll
    var q : sendpoll
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.question, _::from_JV((v as _object)["question"], type<string>))
    move_to_ref(q._options, _::from_JV((v as _object)["_options"], type<array<string>>))
    move_to_ref(q.is_anonymous, _::from_JV((v as _object)["is_anonymous"], type<bool>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.allows_multiple_answers, _::from_JV((v as _object)["allows_multiple_answers"], type<bool>))
    move_to_ref(q.correct_option_id, _::from_JV((v as _object)["correct_option_id"], type<int>))
    move_to_ref(q.explanation, _::from_JV((v as _object)["explanation"], type<string>))
    move_to_ref(q.explanation_parse_mode, _::from_JV((v as _object)["explanation_parse_mode"], type<string>))
    move_to_ref(q.explanation_entities, _::from_JV((v as _object)["explanation_entities"], type<array<messageentity>>))
    move_to_ref(q.open_period, _::from_JV((v as _object)["open_period"], type<int>))
    move_to_ref(q.close_date, _::from_JV((v as _object)["close_date"], type<int>))
    move_to_ref(q.is_closed, _::from_JV((v as _object)["is_closed"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendpoll? ) : sendpoll?
    return null if v==null
    var q = new sendpoll
    *q <- from_JV(v,type<sendpoll>)
    return q

struct senddice
    //!  Use this method to send an animated emoji that will display a random value. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    [[optional]] emoji : string	//! Optional. Emoji on which the dice throw animation is based. Currently, must be one of “”, “”, “”, “”, “”, or “”. Dice can have values 1-6 for “”, “” and “”, values 1-5 for “” and “”, and values 1-64 for “”. Defaults to “”
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:senddice ) : senddice
    var q : senddice
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.emoji, _::from_JV((v as _object)["emoji"], type<string>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:senddice? ) : senddice?
    return null if v==null
    var q = new senddice
    *q <- from_JV(v,type<senddice>)
    return q

struct sendchataction
    //!  Use this method when you need to tell the user that something is happening on the bot's side. The status is set for 5 seconds or less (when a message arrives from your bot, Telegram clients clear its typing status). Returns True on success. We only recommend using this method when a response from the bot will take a noticeable amount of time to arrive.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread; supergroups only
    action : string	//! Required. Type of action to broadcast. Choose one, depending on what the user is about to receive: typing for text messages, upload_photo for photos, record_video or upload_video for videos, record_voice or upload_voice for voice notes, upload_document for general files, choose_sticker for stickers, find_location for location data, record_video_note or upload_video_note for video notes.

def from_JV ( var v:JsonValue?; t:sendchataction ) : sendchataction
    var q : sendchataction
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.action, _::from_JV((v as _object)["action"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendchataction? ) : sendchataction?
    return null if v==null
    var q = new sendchataction
    *q <- from_JV(v,type<sendchataction>)
    return q

struct getuserprofilephotos
    //!  Use this method to get a list of profile pictures for a user. Returns a UserProfilePhotos object.
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] offset : int	//! Optional. Sequential number of the first photo to be returned. By default, all photos are returned.
    [[optional]] limit : int	//! Optional. Limits the number of photos to be retrieved. Values between 1-100 are accepted. Defaults to 100.

def from_JV ( var v:JsonValue?; t:getuserprofilephotos ) : getuserprofilephotos
    var q : getuserprofilephotos
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.offset, _::from_JV((v as _object)["offset"], type<int>))
    move_to_ref(q.limit, _::from_JV((v as _object)["limit"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:getuserprofilephotos? ) : getuserprofilephotos?
    return null if v==null
    var q = new getuserprofilephotos
    *q <- from_JV(v,type<getuserprofilephotos>)
    return q

struct getfile
    //!  Use this method to get basic information about a file and prepare it for downloading. For the moment, bots can download files of up to 20MB in size. On success, a File object is returned. The file can then be downloaded via the link https://api.telegram.org/file/bot<token>/<file_path>, where <file_path> is taken from the response. It is guaranteed that the link will be valid for at least 1 hour. When the link expires, a new one can be requested by calling getFile again.
    file_id : string	//! Required. File identifier to get information about

def from_JV ( var v:JsonValue?; t:getfile ) : getfile
    var q : getfile
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getfile? ) : getfile?
    return null if v==null
    var q = new getfile
    *q <- from_JV(v,type<getfile>)
    return q

struct banchatmember
    //!  Use this method to ban a user in a group, a supergroup or a channel. In the case of supergroups and channels, the user will not be able to return to the chat on their own using invite links, etc., unless unbanned first. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target group or username of the target supergroup or channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] until_date : int	//! Optional. Date when the user will be unbanned, unix time. If user is banned for more than 366 days or less than 30 seconds from the current time they are considered to be banned forever. Applied for supergroups and channels only.
    [[optional]] revoke_messages : bool	//! Optional. Pass True to delete all messages from the chat for the user that is being removed. If False, the user will be able to see messages in the group that were sent before the user was removed. Always True for supergroups and channels.

def from_JV ( var v:JsonValue?; t:banchatmember ) : banchatmember
    var q : banchatmember
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.until_date, _::from_JV((v as _object)["until_date"], type<int>))
    move_to_ref(q.revoke_messages, _::from_JV((v as _object)["revoke_messages"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:banchatmember? ) : banchatmember?
    return null if v==null
    var q = new banchatmember
    *q <- from_JV(v,type<banchatmember>)
    return q

struct unbanchatmember
    //!  Use this method to unban a previously banned user in a supergroup or channel. The user will not return to the group or channel automatically, but will be able to join via link, etc. The bot must be an administrator for this to work. By default, this method guarantees that after the call the user is not a member of the chat, but will be able to join it. So if the user is a member of the chat they will also be removed from the chat. If you don't want this, use the parameter only_if_banned. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target group or username of the target supergroup or channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] only_if_banned : bool	//! Optional. Do nothing if the user is not banned

def from_JV ( var v:JsonValue?; t:unbanchatmember ) : unbanchatmember
    var q : unbanchatmember
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.only_if_banned, _::from_JV((v as _object)["only_if_banned"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:unbanchatmember? ) : unbanchatmember?
    return null if v==null
    var q = new unbanchatmember
    *q <- from_JV(v,type<unbanchatmember>)
    return q

struct restrictchatmember
    //!  Use this method to restrict a user in a supergroup. The bot must be an administrator in the supergroup for this to work and must have the appropriate administrator rights. Pass True for all permissions to lift restrictions from a user. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    user_id : int	//! Required. Unique identifier of the target user
    permissions : chatpermissions	//! Required. A JSON-serialized object for new user permissions
    [[optional]] use_independent_chat_permissions : bool	//! Optional. Pass True if chat permissions are set independently. Otherwise, the can_send_other_messages and can_add_web_page_previews permissions will imply the can_send_messages, can_send_audios, can_send_documents, can_send_photos, can_send_videos, can_send_video_notes, and can_send_voice_notes permissions; the can_send_polls permission will imply the can_send_messages permission.
    [[optional]] until_date : int	//! Optional. Date when restrictions will be lifted for the user, unix time. If user is restricted for more than 366 days or less than 30 seconds from the current time, they are considered to be restricted forever

def from_JV ( var v:JsonValue?; t:restrictchatmember ) : restrictchatmember
    var q : restrictchatmember
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.permissions, _::from_JV((v as _object)["permissions"], type<chatpermissions>))
    move_to_ref(q.use_independent_chat_permissions, _::from_JV((v as _object)["use_independent_chat_permissions"], type<bool>))
    move_to_ref(q.until_date, _::from_JV((v as _object)["until_date"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:restrictchatmember? ) : restrictchatmember?
    return null if v==null
    var q = new restrictchatmember
    *q <- from_JV(v,type<restrictchatmember>)
    return q

struct promotechatmember
    //!  Use this method to promote or demote a user in a supergroup or a channel. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Pass False for all boolean parameters to demote a user. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] is_anonymous : bool	//! Optional. Pass True if the administrator's presence in the chat is hidden
    [[optional]] can_manage_chat : bool	//! Optional. Pass True if the administrator can access the chat event log, chat statistics, message statistics in channels, see channel members, see anonymous administrators in supergroups and ignore slow mode. Implied by any other administrator privilege
    [[optional]] can_post_messages : bool	//! Optional. Pass True if the administrator can create channel posts, channels only
    [[optional]] can_edit_messages : bool	//! Optional. Pass True if the administrator can edit messages of other users and can pin messages, channels only
    [[optional]] can_delete_messages : bool	//! Optional. Pass True if the administrator can delete messages of other users
    [[optional]] can_manage_video_chats : bool	//! Optional. Pass True if the administrator can manage video chats
    [[optional]] can_restrict_members : bool	//! Optional. Pass True if the administrator can restrict, ban or unban chat members
    [[optional]] can_promote_members : bool	//! Optional. Pass True if the administrator can add new administrators with a subset of their own privileges or demote administrators that they have promoted, directly or indirectly (promoted by administrators that were appointed by him)
    [[optional]] can_change_info : bool	//! Optional. Pass True if the administrator can change chat title, photo and other settings
    [[optional]] can_invite_users : bool	//! Optional. Pass True if the administrator can invite new users to the chat
    [[optional]] can_pin_messages : bool	//! Optional. Pass True if the administrator can pin messages, supergroups only
    [[optional]] can_manage_topics : bool	//! Optional. Pass True if the user is allowed to create, rename, close, and reopen forum topics, supergroups only

def from_JV ( var v:JsonValue?; t:promotechatmember ) : promotechatmember
    var q : promotechatmember
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.is_anonymous, _::from_JV((v as _object)["is_anonymous"], type<bool>))
    move_to_ref(q.can_manage_chat, _::from_JV((v as _object)["can_manage_chat"], type<bool>))
    move_to_ref(q.can_post_messages, _::from_JV((v as _object)["can_post_messages"], type<bool>))
    move_to_ref(q.can_edit_messages, _::from_JV((v as _object)["can_edit_messages"], type<bool>))
    move_to_ref(q.can_delete_messages, _::from_JV((v as _object)["can_delete_messages"], type<bool>))
    move_to_ref(q.can_manage_video_chats, _::from_JV((v as _object)["can_manage_video_chats"], type<bool>))
    move_to_ref(q.can_restrict_members, _::from_JV((v as _object)["can_restrict_members"], type<bool>))
    move_to_ref(q.can_promote_members, _::from_JV((v as _object)["can_promote_members"], type<bool>))
    move_to_ref(q.can_change_info, _::from_JV((v as _object)["can_change_info"], type<bool>))
    move_to_ref(q.can_invite_users, _::from_JV((v as _object)["can_invite_users"], type<bool>))
    move_to_ref(q.can_pin_messages, _::from_JV((v as _object)["can_pin_messages"], type<bool>))
    move_to_ref(q.can_manage_topics, _::from_JV((v as _object)["can_manage_topics"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:promotechatmember? ) : promotechatmember?
    return null if v==null
    var q = new promotechatmember
    *q <- from_JV(v,type<promotechatmember>)
    return q

struct setchatadministratorcustomtitle
    //!  Use this method to set a custom title for an administrator in a supergroup promoted by the bot. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    user_id : int	//! Required. Unique identifier of the target user
    custom_title : string	//! Required. New custom title for the administrator; 0-16 characters, emoji are not allowed

def from_JV ( var v:JsonValue?; t:setchatadministratorcustomtitle ) : setchatadministratorcustomtitle
    var q : setchatadministratorcustomtitle
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.custom_title, _::from_JV((v as _object)["custom_title"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchatadministratorcustomtitle? ) : setchatadministratorcustomtitle?
    return null if v==null
    var q = new setchatadministratorcustomtitle
    *q <- from_JV(v,type<setchatadministratorcustomtitle>)
    return q

struct banchatsenderchat
    //!  Use this method to ban a channel chat in a supergroup or a channel. Until the chat is unbanned, the owner of the banned chat won't be able to send messages on behalf of any of their channels. The bot must be an administrator in the supergroup or channel for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    sender_chat_id : int	//! Required. Unique identifier of the target sender chat

def from_JV ( var v:JsonValue?; t:banchatsenderchat ) : banchatsenderchat
    var q : banchatsenderchat
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.sender_chat_id, _::from_JV((v as _object)["sender_chat_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:banchatsenderchat? ) : banchatsenderchat?
    return null if v==null
    var q = new banchatsenderchat
    *q <- from_JV(v,type<banchatsenderchat>)
    return q

struct unbanchatsenderchat
    //!  Use this method to unban a previously banned channel chat in a supergroup or channel. The bot must be an administrator for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    sender_chat_id : int	//! Required. Unique identifier of the target sender chat

def from_JV ( var v:JsonValue?; t:unbanchatsenderchat ) : unbanchatsenderchat
    var q : unbanchatsenderchat
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.sender_chat_id, _::from_JV((v as _object)["sender_chat_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:unbanchatsenderchat? ) : unbanchatsenderchat?
    return null if v==null
    var q = new unbanchatsenderchat
    *q <- from_JV(v,type<unbanchatsenderchat>)
    return q

struct setchatpermissions
    //!  Use this method to set default chat permissions for all members. The bot must be an administrator in the group or a supergroup for this to work and must have the can_restrict_members administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    permissions : chatpermissions	//! Required. A JSON-serialized object for new default chat permissions
    [[optional]] use_independent_chat_permissions : bool	//! Optional. Pass True if chat permissions are set independently. Otherwise, the can_send_other_messages and can_add_web_page_previews permissions will imply the can_send_messages, can_send_audios, can_send_documents, can_send_photos, can_send_videos, can_send_video_notes, and can_send_voice_notes permissions; the can_send_polls permission will imply the can_send_messages permission.

def from_JV ( var v:JsonValue?; t:setchatpermissions ) : setchatpermissions
    var q : setchatpermissions
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.permissions, _::from_JV((v as _object)["permissions"], type<chatpermissions>))
    move_to_ref(q.use_independent_chat_permissions, _::from_JV((v as _object)["use_independent_chat_permissions"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchatpermissions? ) : setchatpermissions?
    return null if v==null
    var q = new setchatpermissions
    *q <- from_JV(v,type<setchatpermissions>)
    return q

struct exportchatinvitelink
    //!  Use this method to generate a new primary invite link for a chat; any previously generated primary link is revoked. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns the new invite link as String on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:exportchatinvitelink ) : exportchatinvitelink
    var q : exportchatinvitelink
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:exportchatinvitelink? ) : exportchatinvitelink?
    return null if v==null
    var q = new exportchatinvitelink
    *q <- from_JV(v,type<exportchatinvitelink>)
    return q

struct createchatinvitelink
    //!  Use this method to create an additional invite link for a chat. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. The link can be revoked using the method revokeChatInviteLink. Returns the new invite link as ChatInviteLink object.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] name : string	//! Optional. Invite link name; 0-32 characters
    [[optional]] expire_date : int	//! Optional. Point in time (Unix timestamp) when the link will expire
    [[optional]] member_limit : int	//! Optional. The maximum number of users that can be members of the chat simultaneously after joining the chat via this invite link; 1-99999
    [[optional]] creates_join_request : bool	//! Optional. True, if users joining the chat via the link need to be approved by chat administrators. If True, member_limit can't be specified

def from_JV ( var v:JsonValue?; t:createchatinvitelink ) : createchatinvitelink
    var q : createchatinvitelink
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.expire_date, _::from_JV((v as _object)["expire_date"], type<int>))
    move_to_ref(q.member_limit, _::from_JV((v as _object)["member_limit"], type<int>))
    move_to_ref(q.creates_join_request, _::from_JV((v as _object)["creates_join_request"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:createchatinvitelink? ) : createchatinvitelink?
    return null if v==null
    var q = new createchatinvitelink
    *q <- from_JV(v,type<createchatinvitelink>)
    return q

struct editchatinvitelink
    //!  Use this method to edit a non-primary invite link created by the bot. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns the edited invite link as a ChatInviteLink object.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    invite_link : string	//! Required. The invite link to edit
    [[optional]] name : string	//! Optional. Invite link name; 0-32 characters
    [[optional]] expire_date : int	//! Optional. Point in time (Unix timestamp) when the link will expire
    [[optional]] member_limit : int	//! Optional. The maximum number of users that can be members of the chat simultaneously after joining the chat via this invite link; 1-99999
    [[optional]] creates_join_request : bool	//! Optional. True, if users joining the chat via the link need to be approved by chat administrators. If True, member_limit can't be specified

def from_JV ( var v:JsonValue?; t:editchatinvitelink ) : editchatinvitelink
    var q : editchatinvitelink
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.invite_link, _::from_JV((v as _object)["invite_link"], type<string>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.expire_date, _::from_JV((v as _object)["expire_date"], type<int>))
    move_to_ref(q.member_limit, _::from_JV((v as _object)["member_limit"], type<int>))
    move_to_ref(q.creates_join_request, _::from_JV((v as _object)["creates_join_request"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:editchatinvitelink? ) : editchatinvitelink?
    return null if v==null
    var q = new editchatinvitelink
    *q <- from_JV(v,type<editchatinvitelink>)
    return q

struct revokechatinvitelink
    //!  Use this method to revoke an invite link created by the bot. If the primary link is revoked, a new link is automatically generated. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns the revoked invite link as ChatInviteLink object.
    chat_id : string	//! Required. Unique identifier of the target chat or username of the target channel (in the format @channelusername)
    invite_link : string	//! Required. The invite link to revoke

def from_JV ( var v:JsonValue?; t:revokechatinvitelink ) : revokechatinvitelink
    var q : revokechatinvitelink
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.invite_link, _::from_JV((v as _object)["invite_link"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:revokechatinvitelink? ) : revokechatinvitelink?
    return null if v==null
    var q = new revokechatinvitelink
    *q <- from_JV(v,type<revokechatinvitelink>)
    return q

struct approvechatjoinrequest
    //!  Use this method to approve a chat join request. The bot must be an administrator in the chat for this to work and must have the can_invite_users administrator right. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user

def from_JV ( var v:JsonValue?; t:approvechatjoinrequest ) : approvechatjoinrequest
    var q : approvechatjoinrequest
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:approvechatjoinrequest? ) : approvechatjoinrequest?
    return null if v==null
    var q = new approvechatjoinrequest
    *q <- from_JV(v,type<approvechatjoinrequest>)
    return q

struct declinechatjoinrequest
    //!  Use this method to decline a chat join request. The bot must be an administrator in the chat for this to work and must have the can_invite_users administrator right. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user

def from_JV ( var v:JsonValue?; t:declinechatjoinrequest ) : declinechatjoinrequest
    var q : declinechatjoinrequest
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:declinechatjoinrequest? ) : declinechatjoinrequest?
    return null if v==null
    var q = new declinechatjoinrequest
    *q <- from_JV(v,type<declinechatjoinrequest>)
    return q

struct setchatphoto
    //!  Use this method to set a new profile photo for the chat. Photos can't be changed for private chats. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    photo : inputfile	//! Required. New chat photo, uploaded using multipart/form-data

def from_JV ( var v:JsonValue?; t:setchatphoto ) : setchatphoto
    var q : setchatphoto
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.photo, _::from_JV((v as _object)["photo"], type<inputfile>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchatphoto? ) : setchatphoto?
    return null if v==null
    var q = new setchatphoto
    *q <- from_JV(v,type<setchatphoto>)
    return q

struct deletechatphoto
    //!  Use this method to delete a chat photo. Photos can't be changed for private chats. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:deletechatphoto ) : deletechatphoto
    var q : deletechatphoto
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletechatphoto? ) : deletechatphoto?
    return null if v==null
    var q = new deletechatphoto
    *q <- from_JV(v,type<deletechatphoto>)
    return q

struct setchattitle
    //!  Use this method to change the title of a chat. Titles can't be changed for private chats. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    title : string	//! Required. New chat title, 1-128 characters

def from_JV ( var v:JsonValue?; t:setchattitle ) : setchattitle
    var q : setchattitle
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchattitle? ) : setchattitle?
    return null if v==null
    var q = new setchattitle
    *q <- from_JV(v,type<setchattitle>)
    return q

struct setchatdescription
    //!  Use this method to change the description of a group, a supergroup or a channel. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] description : string	//! Optional. New chat description, 0-255 characters

def from_JV ( var v:JsonValue?; t:setchatdescription ) : setchatdescription
    var q : setchatdescription
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchatdescription? ) : setchatdescription?
    return null if v==null
    var q = new setchatdescription
    *q <- from_JV(v,type<setchatdescription>)
    return q

struct pinchatmessage
    //!  Use this method to add a message to the list of pinned messages in a chat. If the chat is not a private chat, the bot must be an administrator in the chat for this to work and must have the 'can_pin_messages' administrator right in a supergroup or 'can_edit_messages' administrator right in a channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    message_id : int	//! Required. Identifier of a message to pin
    [[optional]] disable_notification : bool	//! Optional. Pass True if it is not necessary to send a notification to all chat members about the new pinned message. Notifications are always disabled in channels and private chats.

def from_JV ( var v:JsonValue?; t:pinchatmessage ) : pinchatmessage
    var q : pinchatmessage
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:pinchatmessage? ) : pinchatmessage?
    return null if v==null
    var q = new pinchatmessage
    *q <- from_JV(v,type<pinchatmessage>)
    return q

struct unpinchatmessage
    //!  Use this method to remove a message from the list of pinned messages in a chat. If the chat is not a private chat, the bot must be an administrator in the chat for this to work and must have the 'can_pin_messages' administrator right in a supergroup or 'can_edit_messages' administrator right in a channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Identifier of a message to unpin. If not specified, the most recent pinned message (by sending date) will be unpinned.

def from_JV ( var v:JsonValue?; t:unpinchatmessage ) : unpinchatmessage
    var q : unpinchatmessage
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:unpinchatmessage? ) : unpinchatmessage?
    return null if v==null
    var q = new unpinchatmessage
    *q <- from_JV(v,type<unpinchatmessage>)
    return q

struct unpinallchatmessages
    //!  Use this method to clear the list of pinned messages in a chat. If the chat is not a private chat, the bot must be an administrator in the chat for this to work and must have the 'can_pin_messages' administrator right in a supergroup or 'can_edit_messages' administrator right in a channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:unpinallchatmessages ) : unpinallchatmessages
    var q : unpinallchatmessages
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:unpinallchatmessages? ) : unpinallchatmessages?
    return null if v==null
    var q = new unpinallchatmessages
    *q <- from_JV(v,type<unpinallchatmessages>)
    return q

struct leavechat
    //!  Use this method for your bot to leave a group, supergroup or channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:leavechat ) : leavechat
    var q : leavechat
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:leavechat? ) : leavechat?
    return null if v==null
    var q = new leavechat
    *q <- from_JV(v,type<leavechat>)
    return q

struct getchat
    //!  Use this method to get up to date information about the chat (current name of the user for one-on-one conversations, current username of a user, group or channel, etc.). Returns a Chat object on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:getchat ) : getchat
    var q : getchat
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getchat? ) : getchat?
    return null if v==null
    var q = new getchat
    *q <- from_JV(v,type<getchat>)
    return q

struct getchatadministrators
    //!  Use this method to get a list of administrators in a chat, which aren't bots. Returns an Array of ChatMember objects.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:getchatadministrators ) : getchatadministrators
    var q : getchatadministrators
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getchatadministrators? ) : getchatadministrators?
    return null if v==null
    var q = new getchatadministrators
    *q <- from_JV(v,type<getchatadministrators>)
    return q

struct getchatmembercount
    //!  Use this method to get the number of members in a chat. Returns Int on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

def from_JV ( var v:JsonValue?; t:getchatmembercount ) : getchatmembercount
    var q : getchatmembercount
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getchatmembercount? ) : getchatmembercount?
    return null if v==null
    var q = new getchatmembercount
    *q <- from_JV(v,type<getchatmembercount>)
    return q

struct getchatmember
    //!  Use this method to get information about a member of a chat. The method is only guaranteed to work for other users if the bot is an administrator in the chat. Returns a ChatMember object on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user

def from_JV ( var v:JsonValue?; t:getchatmember ) : getchatmember
    var q : getchatmember
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:getchatmember? ) : getchatmember?
    return null if v==null
    var q = new getchatmember
    *q <- from_JV(v,type<getchatmember>)
    return q

struct setchatstickerset
    //!  Use this method to set a new group sticker set for a supergroup. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Use the field can_set_sticker_set optionally returned in getChat requests to check if the bot can use this method. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    sticker_set_name : string	//! Required. Name of the sticker set to be set as the group sticker set

def from_JV ( var v:JsonValue?; t:setchatstickerset ) : setchatstickerset
    var q : setchatstickerset
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.sticker_set_name, _::from_JV((v as _object)["sticker_set_name"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchatstickerset? ) : setchatstickerset?
    return null if v==null
    var q = new setchatstickerset
    *q <- from_JV(v,type<setchatstickerset>)
    return q

struct deletechatstickerset
    //!  Use this method to delete a group sticker set from a supergroup. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Use the field can_set_sticker_set optionally returned in getChat requests to check if the bot can use this method. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:deletechatstickerset ) : deletechatstickerset
    var q : deletechatstickerset
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletechatstickerset? ) : deletechatstickerset?
    return null if v==null
    var q = new deletechatstickerset
    *q <- from_JV(v,type<deletechatstickerset>)
    return q

struct getforumtopiciconstickers
    //!  Use this method to get custom emoji stickers, which can be used as a forum topic icon by any user. Requires no parameters. Returns an Array of Sticker objects.
    object_getforumtopiciconstickers_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:getforumtopiciconstickers ) : getforumtopiciconstickers
    var q : getforumtopiciconstickers
    return <- q

def from_JV ( var v:JsonValue?; t:getforumtopiciconstickers? ) : getforumtopiciconstickers?
    return null if v==null
    var q = new getforumtopiciconstickers
    *q <- from_JV(v,type<getforumtopiciconstickers>)
    return q

struct createforumtopic
    //!  Use this method to create a topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. Returns information about the created topic as a ForumTopic object.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    name : string	//! Required. Topic name, 1-128 characters
    [[optional]] icon_color : int	//! Optional. Color of the topic icon in RGB format. Currently, must be one of 7322096 (0x6FB9F0), 16766590 (0xFFD67E), 13338331 (0xCB86DB), 9367192 (0x8EEE98), 16749490 (0xFF93B2), or 16478047 (0xFB6F5F)
    [[optional]] icon_custom_emoji_id : string	//! Optional. Unique identifier of the custom emoji shown as the topic icon. Use getForumTopicIconStickers to get all allowed custom emoji identifiers.

def from_JV ( var v:JsonValue?; t:createforumtopic ) : createforumtopic
    var q : createforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.icon_color, _::from_JV((v as _object)["icon_color"], type<int>))
    move_to_ref(q.icon_custom_emoji_id, _::from_JV((v as _object)["icon_custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:createforumtopic? ) : createforumtopic?
    return null if v==null
    var q = new createforumtopic
    *q <- from_JV(v,type<createforumtopic>)
    return q

struct editforumtopic
    //!  Use this method to edit name and icon of a topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have can_manage_topics administrator rights, unless it is the creator of the topic. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic
    [[optional]] name : string	//! Optional. New topic name, 0-128 characters. If not specified or empty, the current name of the topic will be kept
    [[optional]] icon_custom_emoji_id : string	//! Optional. New unique identifier of the custom emoji shown as the topic icon. Use getForumTopicIconStickers to get all allowed custom emoji identifiers. Pass an empty string to remove the icon. If not specified, the current icon will be kept

def from_JV ( var v:JsonValue?; t:editforumtopic ) : editforumtopic
    var q : editforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.icon_custom_emoji_id, _::from_JV((v as _object)["icon_custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:editforumtopic? ) : editforumtopic?
    return null if v==null
    var q = new editforumtopic
    *q <- from_JV(v,type<editforumtopic>)
    return q

struct closeforumtopic
    //!  Use this method to close an open topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights, unless it is the creator of the topic. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

def from_JV ( var v:JsonValue?; t:closeforumtopic ) : closeforumtopic
    var q : closeforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:closeforumtopic? ) : closeforumtopic?
    return null if v==null
    var q = new closeforumtopic
    *q <- from_JV(v,type<closeforumtopic>)
    return q

struct reopenforumtopic
    //!  Use this method to reopen a closed topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights, unless it is the creator of the topic. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

def from_JV ( var v:JsonValue?; t:reopenforumtopic ) : reopenforumtopic
    var q : reopenforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:reopenforumtopic? ) : reopenforumtopic?
    return null if v==null
    var q = new reopenforumtopic
    *q <- from_JV(v,type<reopenforumtopic>)
    return q

struct deleteforumtopic
    //!  Use this method to delete a forum topic along with all its messages in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_delete_messages administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

def from_JV ( var v:JsonValue?; t:deleteforumtopic ) : deleteforumtopic
    var q : deleteforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:deleteforumtopic? ) : deleteforumtopic?
    return null if v==null
    var q = new deleteforumtopic
    *q <- from_JV(v,type<deleteforumtopic>)
    return q

struct unpinallforumtopicmessages
    //!  Use this method to clear the list of pinned messages in a forum topic. The bot must be an administrator in the chat for this to work and must have the can_pin_messages administrator right in the supergroup. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

def from_JV ( var v:JsonValue?; t:unpinallforumtopicmessages ) : unpinallforumtopicmessages
    var q : unpinallforumtopicmessages
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:unpinallforumtopicmessages? ) : unpinallforumtopicmessages?
    return null if v==null
    var q = new unpinallforumtopicmessages
    *q <- from_JV(v,type<unpinallforumtopicmessages>)
    return q

struct editgeneralforumtopic
    //!  Use this method to edit the name of the 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have can_manage_topics administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    name : string	//! Required. New topic name, 1-128 characters

def from_JV ( var v:JsonValue?; t:editgeneralforumtopic ) : editgeneralforumtopic
    var q : editgeneralforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:editgeneralforumtopic? ) : editgeneralforumtopic?
    return null if v==null
    var q = new editgeneralforumtopic
    *q <- from_JV(v,type<editgeneralforumtopic>)
    return q

struct closegeneralforumtopic
    //!  Use this method to close an open 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:closegeneralforumtopic ) : closegeneralforumtopic
    var q : closegeneralforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:closegeneralforumtopic? ) : closegeneralforumtopic?
    return null if v==null
    var q = new closegeneralforumtopic
    *q <- from_JV(v,type<closegeneralforumtopic>)
    return q

struct reopengeneralforumtopic
    //!  Use this method to reopen a closed 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. The topic will be automatically unhidden if it was hidden. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:reopengeneralforumtopic ) : reopengeneralforumtopic
    var q : reopengeneralforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:reopengeneralforumtopic? ) : reopengeneralforumtopic?
    return null if v==null
    var q = new reopengeneralforumtopic
    *q <- from_JV(v,type<reopengeneralforumtopic>)
    return q

struct hidegeneralforumtopic
    //!  Use this method to hide the 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. The topic will be automatically closed if it was open. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:hidegeneralforumtopic ) : hidegeneralforumtopic
    var q : hidegeneralforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:hidegeneralforumtopic? ) : hidegeneralforumtopic?
    return null if v==null
    var q = new hidegeneralforumtopic
    *q <- from_JV(v,type<hidegeneralforumtopic>)
    return q

struct unhidegeneralforumtopic
    //!  Use this method to unhide the 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

def from_JV ( var v:JsonValue?; t:unhidegeneralforumtopic ) : unhidegeneralforumtopic
    var q : unhidegeneralforumtopic
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:unhidegeneralforumtopic? ) : unhidegeneralforumtopic?
    return null if v==null
    var q = new unhidegeneralforumtopic
    *q <- from_JV(v,type<unhidegeneralforumtopic>)
    return q

struct answercallbackquery
    //!  Use this method to send answers to callback queries sent from inline keyboards. The answer will be displayed to the user as a notification at the top of the chat screen or as an alert. On success, True is returned.
    callback_query_id : string	//! Required. Unique identifier for the query to be answered
    [[optional]] text : string	//! Optional. Text of the notification. If not specified, nothing will be shown to the user, 0-200 characters
    [[optional]] show_alert : bool	//! Optional. If True, an alert will be shown by the client instead of a notification at the top of the chat screen. Defaults to false.
    [[optional]] url : string	//! Optional. URL that will be opened by the user's client. If you have created a Game and accepted the conditions via @BotFather, specify the URL that opens your game - note that this will only work if the query comes from a callback_game button. Otherwise, you may use links like t.me/your_bot?start=XXXX that open your bot with a parameter.
    [[optional]] cache_time : int	//! Optional. The maximum amount of time in seconds that the result of the callback query may be cached client-side. Telegram apps will support caching starting in version 3.14. Defaults to 0.

def from_JV ( var v:JsonValue?; t:answercallbackquery ) : answercallbackquery
    var q : answercallbackquery
    move_to_ref(q.callback_query_id, _::from_JV((v as _object)["callback_query_id"], type<string>))
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.show_alert, _::from_JV((v as _object)["show_alert"], type<bool>))
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.cache_time, _::from_JV((v as _object)["cache_time"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:answercallbackquery? ) : answercallbackquery?
    return null if v==null
    var q = new answercallbackquery
    *q <- from_JV(v,type<answercallbackquery>)
    return q

struct setmycommands
    //!  Use this method to change the list of the bot's commands. See this manual for more details about bot commands. Returns True on success.
    commands : array<botcommand>	//! Required. A JSON-serialized list of bot commands to be set as the list of the bot's commands. At most 100 commands can be specified.
    [[optional]] scope : botcommandscope?	//! Optional. A JSON-serialized object, describing scope of users for which the commands are relevant. Defaults to BotCommandScopeDefault.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code. If empty, commands will be applied to all users from the given scope, for whose language there are no dedicated commands

def from_JV ( var v:JsonValue?; t:setmycommands ) : setmycommands
    var q : setmycommands
    move_to_ref(q.commands, _::from_JV((v as _object)["commands"], type<array<botcommand>>))
    move_to_ref(q.scope, _::from_JV((v as _object)["scope"], type<botcommandscope?>))
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setmycommands? ) : setmycommands?
    return null if v==null
    var q = new setmycommands
    *q <- from_JV(v,type<setmycommands>)
    return q

struct deletemycommands
    //!  Use this method to delete the list of the bot's commands for the given scope and user language. After deletion, higher level commands will be shown to affected users. Returns True on success.
    [[optional]] scope : botcommandscope?	//! Optional. A JSON-serialized object, describing scope of users for which the commands are relevant. Defaults to BotCommandScopeDefault.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code. If empty, commands will be applied to all users from the given scope, for whose language there are no dedicated commands

def from_JV ( var v:JsonValue?; t:deletemycommands ) : deletemycommands
    var q : deletemycommands
    move_to_ref(q.scope, _::from_JV((v as _object)["scope"], type<botcommandscope?>))
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletemycommands? ) : deletemycommands?
    return null if v==null
    var q = new deletemycommands
    *q <- from_JV(v,type<deletemycommands>)
    return q

struct getmycommands
    //!  Use this method to get the current list of the bot's commands for the given scope and user language. Returns an Array of BotCommand objects. If commands aren't set, an empty list is returned.
    [[optional]] scope : botcommandscope?	//! Optional. A JSON-serialized object, describing scope of users. Defaults to BotCommandScopeDefault.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code or an empty string

def from_JV ( var v:JsonValue?; t:getmycommands ) : getmycommands
    var q : getmycommands
    move_to_ref(q.scope, _::from_JV((v as _object)["scope"], type<botcommandscope?>))
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getmycommands? ) : getmycommands?
    return null if v==null
    var q = new getmycommands
    *q <- from_JV(v,type<getmycommands>)
    return q

struct setmydescription
    //!  Use this method to change the bot's description, which is shown in the chat with the bot if the chat is empty. Returns True on success.
    [[optional]] description : string	//! Optional. New bot description; 0-512 characters. Pass an empty string to remove the dedicated description for the given language.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code. If empty, the description will be applied to all users for whose language there is no dedicated description.

def from_JV ( var v:JsonValue?; t:setmydescription ) : setmydescription
    var q : setmydescription
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setmydescription? ) : setmydescription?
    return null if v==null
    var q = new setmydescription
    *q <- from_JV(v,type<setmydescription>)
    return q

struct getmydescription
    //!  Use this method to get the current bot description for the given user language. Returns BotDescription on success.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code or an empty string

def from_JV ( var v:JsonValue?; t:getmydescription ) : getmydescription
    var q : getmydescription
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getmydescription? ) : getmydescription?
    return null if v==null
    var q = new getmydescription
    *q <- from_JV(v,type<getmydescription>)
    return q

struct setmyshortdescription
    //!  Use this method to change the bot's short description, which is shown on the bot's profile page and is sent together with the link when users share the bot. Returns True on success.
    [[optional]] short_description : string	//! Optional. New short description for the bot; 0-120 characters. Pass an empty string to remove the dedicated short description for the given language.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code. If empty, the short description will be applied to all users for whose language there is no dedicated short description.

def from_JV ( var v:JsonValue?; t:setmyshortdescription ) : setmyshortdescription
    var q : setmyshortdescription
    move_to_ref(q.short_description, _::from_JV((v as _object)["short_description"], type<string>))
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setmyshortdescription? ) : setmyshortdescription?
    return null if v==null
    var q = new setmyshortdescription
    *q <- from_JV(v,type<setmyshortdescription>)
    return q

struct getmyshortdescription
    //!  Use this method to get the current bot short description for the given user language. Returns BotShortDescription on success.
    [[optional]] language_code : string	//! Optional. A two-letter ISO 639-1 language code or an empty string

def from_JV ( var v:JsonValue?; t:getmyshortdescription ) : getmyshortdescription
    var q : getmyshortdescription
    move_to_ref(q.language_code, _::from_JV((v as _object)["language_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getmyshortdescription? ) : getmyshortdescription?
    return null if v==null
    var q = new getmyshortdescription
    *q <- from_JV(v,type<getmyshortdescription>)
    return q

struct setchatmenubutton
    //!  Use this method to change the bot's menu button in a private chat, or the default menu button. Returns True on success.
    [[optional]] chat_id : int	//! Optional. Unique identifier for the target private chat. If not specified, default bot's menu button will be changed
    [[optional]] menu_button : menubutton?	//! Optional. A JSON-serialized object for the bot's new menu button. Defaults to MenuButtonDefault

def from_JV ( var v:JsonValue?; t:setchatmenubutton ) : setchatmenubutton
    var q : setchatmenubutton
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<int>))
    move_to_ref(q.menu_button, _::from_JV((v as _object)["menu_button"], type<menubutton?>))
    return <- q

def from_JV ( var v:JsonValue?; t:setchatmenubutton? ) : setchatmenubutton?
    return null if v==null
    var q = new setchatmenubutton
    *q <- from_JV(v,type<setchatmenubutton>)
    return q

struct getchatmenubutton
    //!  Use this method to get the current value of the bot's menu button in a private chat, or the default menu button. Returns MenuButton on success.
    [[optional]] chat_id : int	//! Optional. Unique identifier for the target private chat. If not specified, default bot's menu button will be returned

def from_JV ( var v:JsonValue?; t:getchatmenubutton ) : getchatmenubutton
    var q : getchatmenubutton
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:getchatmenubutton? ) : getchatmenubutton?
    return null if v==null
    var q = new getchatmenubutton
    *q <- from_JV(v,type<getchatmenubutton>)
    return q

struct setmydefaultadministratorrights
    //!  Use this method to change the default administrator rights requested by the bot when it's added as an administrator to groups or channels. These rights will be suggested to users, but they are free to modify the list before adding the bot. Returns True on success.
    [[optional]] rights : chatadministratorrights?	//! Optional. A JSON-serialized object describing new default administrator rights. If not specified, the default administrator rights will be cleared.
    [[optional]] for_channels : bool	//! Optional. Pass True to change the default administrator rights of the bot in channels. Otherwise, the default administrator rights of the bot for groups and supergroups will be changed.

def from_JV ( var v:JsonValue?; t:setmydefaultadministratorrights ) : setmydefaultadministratorrights
    var q : setmydefaultadministratorrights
    move_to_ref(q.rights, _::from_JV((v as _object)["rights"], type<chatadministratorrights?>))
    move_to_ref(q.for_channels, _::from_JV((v as _object)["for_channels"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:setmydefaultadministratorrights? ) : setmydefaultadministratorrights?
    return null if v==null
    var q = new setmydefaultadministratorrights
    *q <- from_JV(v,type<setmydefaultadministratorrights>)
    return q

struct getmydefaultadministratorrights
    //!  Use this method to get the current default administrator rights of the bot. Returns ChatAdministratorRights on success.
    [[optional]] for_channels : bool	//! Optional. Pass True to get default administrator rights of the bot in channels. Otherwise, default administrator rights of the bot for groups and supergroups will be returned.

def from_JV ( var v:JsonValue?; t:getmydefaultadministratorrights ) : getmydefaultadministratorrights
    var q : getmydefaultadministratorrights
    move_to_ref(q.for_channels, _::from_JV((v as _object)["for_channels"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:getmydefaultadministratorrights? ) : getmydefaultadministratorrights?
    return null if v==null
    var q = new getmydefaultadministratorrights
    *q <- from_JV(v,type<getmydefaultadministratorrights>)
    return q

struct inline_mode_methods
    //!  Methods and objects used in the inline mode are described in the Inline mode section.
    object_inline_mode_methods_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:inline_mode_methods ) : inline_mode_methods
    var q : inline_mode_methods
    return <- q

def from_JV ( var v:JsonValue?; t:inline_mode_methods? ) : inline_mode_methods?
    return null if v==null
    var q = new inline_mode_methods
    *q <- from_JV(v,type<inline_mode_methods>)
    return q

struct editmessagetext
    //!  Use this method to edit text and game messages. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    text : string	//! Required. New text of the message, 1-4096 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the message text. See formatting options for more details.
    [[optional]] entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in message text, which can be specified instead of parse_mode
    [[optional]] disable_web_page_preview : bool	//! Optional. Disables link previews for links in this message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard.

def from_JV ( var v:JsonValue?; t:editmessagetext ) : editmessagetext
    var q : editmessagetext
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.entities, _::from_JV((v as _object)["entities"], type<array<messageentity>>))
    move_to_ref(q.disable_web_page_preview, _::from_JV((v as _object)["disable_web_page_preview"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:editmessagetext? ) : editmessagetext?
    return null if v==null
    var q = new editmessagetext
    *q <- from_JV(v,type<editmessagetext>)
    return q

struct editmessagecaption
    //!  Use this method to edit captions of messages. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    [[optional]] caption : string	//! Optional. New caption of the message, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard.

def from_JV ( var v:JsonValue?; t:editmessagecaption ) : editmessagecaption
    var q : editmessagecaption
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:editmessagecaption? ) : editmessagecaption?
    return null if v==null
    var q = new editmessagecaption
    *q <- from_JV(v,type<editmessagecaption>)
    return q

struct editmessagemedia
    //!  Use this method to edit animation, audio, document, photo, or video messages. If a message is part of a message album, then it can be edited only to an audio for audio albums, only to a document for document albums and to a photo or a video otherwise. When an inline message is edited, a new file can't be uploaded; use a previously uploaded file via its file_id or specify a URL. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    media : inputmedia	//! Required. A JSON-serialized object for a new media content of the message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new inline keyboard.

def from_JV ( var v:JsonValue?; t:editmessagemedia ) : editmessagemedia
    var q : editmessagemedia
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.media, _::from_JV((v as _object)["media"], type<inputmedia>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:editmessagemedia? ) : editmessagemedia?
    return null if v==null
    var q = new editmessagemedia
    *q <- from_JV(v,type<editmessagemedia>)
    return q

struct editmessagelivelocation
    //!  Use this method to edit live location messages. A location can be edited until its live_period expires or editing is explicitly disabled by a call to stopMessageLiveLocation. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    latitude : float	//! Required. Latitude of new location
    longitude : float	//! Required. Longitude of new location
    [[optional]] horizontal_accuracy : float	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] heading : int	//! Optional. Direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int	//! Optional. The maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new inline keyboard.

def from_JV ( var v:JsonValue?; t:editmessagelivelocation ) : editmessagelivelocation
    var q : editmessagelivelocation
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.horizontal_accuracy, _::from_JV((v as _object)["horizontal_accuracy"], type<float>))
    move_to_ref(q.heading, _::from_JV((v as _object)["heading"], type<int>))
    move_to_ref(q.proximity_alert_radius, _::from_JV((v as _object)["proximity_alert_radius"], type<int>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:editmessagelivelocation? ) : editmessagelivelocation?
    return null if v==null
    var q = new editmessagelivelocation
    *q <- from_JV(v,type<editmessagelivelocation>)
    return q

struct stopmessagelivelocation
    //!  Use this method to stop updating a live location message before live_period expires. On success, if the message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the message with live location to stop
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new inline keyboard.

def from_JV ( var v:JsonValue?; t:stopmessagelivelocation ) : stopmessagelivelocation
    var q : stopmessagelivelocation
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:stopmessagelivelocation? ) : stopmessagelivelocation?
    return null if v==null
    var q = new stopmessagelivelocation
    *q <- from_JV(v,type<stopmessagelivelocation>)
    return q

struct editmessagereplymarkup
    //!  Use this method to edit only the reply markup of messages. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard.

def from_JV ( var v:JsonValue?; t:editmessagereplymarkup ) : editmessagereplymarkup
    var q : editmessagereplymarkup
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:editmessagereplymarkup? ) : editmessagereplymarkup?
    return null if v==null
    var q = new editmessagereplymarkup
    *q <- from_JV(v,type<editmessagereplymarkup>)
    return q

struct stoppoll
    //!  Use this method to stop a poll which was sent by the bot. On success, the stopped Poll is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    message_id : int	//! Required. Identifier of the original message with the poll
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new message inline keyboard.

def from_JV ( var v:JsonValue?; t:stoppoll ) : stoppoll
    var q : stoppoll
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:stoppoll? ) : stoppoll?
    return null if v==null
    var q = new stoppoll
    *q <- from_JV(v,type<stoppoll>)
    return q

struct deletemessage
    //!  Use this method to delete a message, including service messages, with the following limitations: - A message can only be deleted if it was sent less than 48 hours ago. - Service messages about a supergroup, channel, or forum topic creation can't be deleted. - A dice message in a private chat can only be deleted if it was sent more than 24 hours ago. - Bots can delete outgoing messages in private chats, groups, and supergroups. - Bots can delete incoming messages in private chats. - Bots granted can_post_messages permissions can delete outgoing messages in channels. - If the bot is an administrator of a group, it can delete any message there. - If the bot has can_delete_messages permission in a supergroup or a channel, it can delete any message there. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    message_id : int	//! Required. Identifier of the message to delete

def from_JV ( var v:JsonValue?; t:deletemessage ) : deletemessage
    var q : deletemessage
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletemessage? ) : deletemessage?
    return null if v==null
    var q = new deletemessage
    *q <- from_JV(v,type<deletemessage>)
    return q

struct sticker
    //!  This object represents a sticker.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    _type : string	//! Type of the sticker, currently one of “regular”, “mask”, “custom_emoji”. The type of the sticker is independent from its format, which is determined by the fields is_animated and is_video.
    width : int	//! Sticker width
    height : int	//! Sticker height
    is_animated : bool	//! True, if the sticker is animated
    is_video : bool	//! True, if the sticker is a video sticker
    [[optional]] thumbnail : photosize?	//! Optional. Sticker thumbnail in the .WEBP or .JPG format
    [[optional]] emoji : string	//! Optional. Emoji associated with the sticker
    [[optional]] set_name : string	//! Optional. Name of the sticker set to which the sticker belongs
    [[optional]] premium_animation : file?	//! Optional. For premium regular stickers, premium animation for the sticker
    [[optional]] mask_position : maskposition?	//! Optional. For mask stickers, the position where the mask should be placed
    [[optional]] custom_emoji_id : string	//! Optional. For custom emoji stickers, unique identifier of the custom emoji
    [[optional]] needs_repainting : bool	//! Optional. True, if the sticker must be repainted to a text color in messages, the color of the Telegram Premium badge in emoji status, white color on chat photos, or another appropriate color in other places
    [[optional]] file_size : int	//! Optional. File size in bytes

def from_JV ( var v:JsonValue?; t:sticker ) : sticker
    var q : sticker
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.width, _::from_JV((v as _object)["width"], type<int>))
    move_to_ref(q.height, _::from_JV((v as _object)["height"], type<int>))
    move_to_ref(q.is_animated, _::from_JV((v as _object)["is_animated"], type<bool>))
    move_to_ref(q.is_video, _::from_JV((v as _object)["is_video"], type<bool>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    move_to_ref(q.emoji, _::from_JV((v as _object)["emoji"], type<string>))
    move_to_ref(q.set_name, _::from_JV((v as _object)["set_name"], type<string>))
    move_to_ref(q.premium_animation, _::from_JV((v as _object)["premium_animation"], type<file?>))
    move_to_ref(q.mask_position, _::from_JV((v as _object)["mask_position"], type<maskposition?>))
    move_to_ref(q.custom_emoji_id, _::from_JV((v as _object)["custom_emoji_id"], type<string>))
    move_to_ref(q.needs_repainting, _::from_JV((v as _object)["needs_repainting"], type<bool>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:sticker? ) : sticker?
    return null if v==null
    var q = new sticker
    *q <- from_JV(v,type<sticker>)
    return q

struct stickerset
    //!  This object represents a sticker set.
    name : string	//! Sticker set name
    title : string	//! Sticker set title
    sticker_type : string	//! Type of stickers in the set, currently one of “regular”, “mask”, “custom_emoji”
    is_animated : bool	//! True, if the sticker set contains animated stickers
    is_video : bool	//! True, if the sticker set contains video stickers
    stickers : array<sticker>	//! List of all set stickers
    [[optional]] thumbnail : photosize?	//! Optional. Sticker set thumbnail in the .WEBP, .TGS, or .WEBM format

def from_JV ( var v:JsonValue?; t:stickerset ) : stickerset
    var q : stickerset
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.sticker_type, _::from_JV((v as _object)["sticker_type"], type<string>))
    move_to_ref(q.is_animated, _::from_JV((v as _object)["is_animated"], type<bool>))
    move_to_ref(q.is_video, _::from_JV((v as _object)["is_video"], type<bool>))
    move_to_ref(q.stickers, _::from_JV((v as _object)["stickers"], type<array<sticker>>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<photosize?>))
    return <- q

def from_JV ( var v:JsonValue?; t:stickerset? ) : stickerset?
    return null if v==null
    var q = new stickerset
    *q <- from_JV(v,type<stickerset>)
    return q

struct maskposition
    //!  This object describes the position on faces where a mask should be placed by default.
    point : string	//! The part of the face relative to which the mask should be placed. One of “forehead”, “eyes”, “mouth”, or “chin”.
    x_shift : float	//! Shift by X-axis measured in widths of the mask scaled to the face size, from left to right. For example, choosing -1.0 will place mask just to the left of the default mask position.
    y_shift : float	//! Shift by Y-axis measured in heights of the mask scaled to the face size, from top to bottom. For example, 1.0 will place the mask just below the default mask position.
    scale : float	//! Mask scaling coefficient. For example, 2.0 means double size.

def from_JV ( var v:JsonValue?; t:maskposition ) : maskposition
    var q : maskposition
    move_to_ref(q.point, _::from_JV((v as _object)["point"], type<string>))
    move_to_ref(q.x_shift, _::from_JV((v as _object)["x_shift"], type<float>))
    move_to_ref(q.y_shift, _::from_JV((v as _object)["y_shift"], type<float>))
    move_to_ref(q.scale, _::from_JV((v as _object)["scale"], type<float>))
    return <- q

def from_JV ( var v:JsonValue?; t:maskposition? ) : maskposition?
    return null if v==null
    var q = new maskposition
    *q <- from_JV(v,type<maskposition>)
    return q

struct inputsticker
    //!  This object describes a sticker to be added to a sticker set.
    sticker : inputfile	//! The added sticker. Pass a file_id as a String to send a file that already exists on the Telegram servers, pass an HTTP URL as a String for Telegram to get a file from the Internet, upload a new one using multipart/form-data, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. Animated and video stickers can't be uploaded via HTTP URL. More information on Sending Files »
    emoji_list : array<string>	//! List of 1-20 emoji associated with the sticker
    [[optional]] mask_position : maskposition?	//! Optional. Position where the mask should be placed on faces. For “mask” stickers only.
    [[optional]] keywords : array<string>	//! Optional. List of 0-20 search keywords for the sticker with total length of up to 64 characters. For “regular” and “custom_emoji” stickers only.

def from_JV ( var v:JsonValue?; t:inputsticker ) : inputsticker
    var q : inputsticker
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<inputfile>))
    move_to_ref(q.emoji_list, _::from_JV((v as _object)["emoji_list"], type<array<string>>))
    move_to_ref(q.mask_position, _::from_JV((v as _object)["mask_position"], type<maskposition?>))
    move_to_ref(q.keywords, _::from_JV((v as _object)["keywords"], type<array<string>>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputsticker? ) : inputsticker?
    return null if v==null
    var q = new inputsticker
    *q <- from_JV(v,type<inputsticker>)
    return q

struct sendsticker
    //!  Use this method to send static .WEBP, animated .TGS, or video .WEBM stickers. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    sticker : inputfile	//! Required. Sticker to send. Pass a file_id as String to send a file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a .WEBP sticker from the Internet, or upload a new .WEBP or .TGS sticker using multipart/form-data. More information on Sending Files ». Video stickers can only be sent by a file_id. Animated stickers can't be sent via an HTTP URL.
    [[optional]] emoji : string	//! Optional. Emoji associated with the sticker; only for just uploaded stickers
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

def from_JV ( var v:JsonValue?; t:sendsticker ) : sendsticker
    var q : sendsticker
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<inputfile>))
    move_to_ref(q.emoji, _::from_JV((v as _object)["emoji"], type<string>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendsticker? ) : sendsticker?
    return null if v==null
    var q = new sendsticker
    *q <- from_JV(v,type<sendsticker>)
    return q

struct getstickerset
    //!  Use this method to get a sticker set. On success, a StickerSet object is returned.
    name : string	//! Required. Name of the sticker set

def from_JV ( var v:JsonValue?; t:getstickerset ) : getstickerset
    var q : getstickerset
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getstickerset? ) : getstickerset?
    return null if v==null
    var q = new getstickerset
    *q <- from_JV(v,type<getstickerset>)
    return q

struct getcustomemojistickers
    //!  Use this method to get information about custom emoji stickers by their identifiers. Returns an Array of Sticker objects.
    custom_emoji_ids : array<string>	//! Required. List of custom emoji identifiers. At most 200 custom emoji identifiers can be specified.

def from_JV ( var v:JsonValue?; t:getcustomemojistickers ) : getcustomemojistickers
    var q : getcustomemojistickers
    move_to_ref(q.custom_emoji_ids, _::from_JV((v as _object)["custom_emoji_ids"], type<array<string>>))
    return <- q

def from_JV ( var v:JsonValue?; t:getcustomemojistickers? ) : getcustomemojistickers?
    return null if v==null
    var q = new getcustomemojistickers
    *q <- from_JV(v,type<getcustomemojistickers>)
    return q

struct uploadstickerfile
    //!  Use this method to upload a file with a sticker for later use in the createNewStickerSet and addStickerToSet methods (the file can be used multiple times). Returns the uploaded File on success.
    user_id : int	//! Required. User identifier of sticker file owner
    sticker : inputfile	//! Required. A file with the sticker in .WEBP, .PNG, .TGS, or .WEBM format. See https://core.telegram.org/stickers for technical requirements. More information on Sending Files »
    sticker_format : string	//! Required. Format of the sticker, must be one of “static”, “animated”, “video”

def from_JV ( var v:JsonValue?; t:uploadstickerfile ) : uploadstickerfile
    var q : uploadstickerfile
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<inputfile>))
    move_to_ref(q.sticker_format, _::from_JV((v as _object)["sticker_format"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:uploadstickerfile? ) : uploadstickerfile?
    return null if v==null
    var q = new uploadstickerfile
    *q <- from_JV(v,type<uploadstickerfile>)
    return q

struct createnewstickerset
    //!  Use this method to create a new sticker set owned by a user. The bot will be able to edit the sticker set thus created. Returns True on success.
    user_id : int	//! Required. User identifier of created sticker set owner
    name : string	//! Required. Short name of sticker set, to be used in t.me/addstickers/ URLs (e.g., animals). Can contain only English letters, digits and underscores. Must begin with a letter, can't contain consecutive underscores and must end in "_by_<bot_username>". <bot_username> is case insensitive. 1-64 characters.
    title : string	//! Required. Sticker set title, 1-64 characters
    stickers : array<inputsticker>	//! Required. A JSON-serialized list of 1-50 initial stickers to be added to the sticker set
    sticker_format : string	//! Required. Format of stickers in the set, must be one of “static”, “animated”, “video”
    [[optional]] sticker_type : string	//! Optional. Type of stickers in the set, pass “regular”, “mask”, or “custom_emoji”. By default, a regular sticker set is created.
    [[optional]] needs_repainting : bool	//! Optional. Pass True if stickers in the sticker set must be repainted to the color of text when used in messages, the accent color if used as emoji status, white on chat photos, or another appropriate color based on context; for custom emoji sticker sets only

def from_JV ( var v:JsonValue?; t:createnewstickerset ) : createnewstickerset
    var q : createnewstickerset
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.stickers, _::from_JV((v as _object)["stickers"], type<array<inputsticker>>))
    move_to_ref(q.sticker_format, _::from_JV((v as _object)["sticker_format"], type<string>))
    move_to_ref(q.sticker_type, _::from_JV((v as _object)["sticker_type"], type<string>))
    move_to_ref(q.needs_repainting, _::from_JV((v as _object)["needs_repainting"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:createnewstickerset? ) : createnewstickerset?
    return null if v==null
    var q = new createnewstickerset
    *q <- from_JV(v,type<createnewstickerset>)
    return q

struct addstickertoset
    //!  Use this method to add a new sticker to a set created by the bot. The format of the added sticker must match the format of the other stickers in the set. Emoji sticker sets can have up to 200 stickers. Animated and video sticker sets can have up to 50 stickers. Static sticker sets can have up to 120 stickers. Returns True on success.
    user_id : int	//! Required. User identifier of sticker set owner
    name : string	//! Required. Sticker set name
    sticker : inputsticker	//! Required. A JSON-serialized object with information about the added sticker. If exactly the same sticker had already been added to the set, then the set isn't changed.

def from_JV ( var v:JsonValue?; t:addstickertoset ) : addstickertoset
    var q : addstickertoset
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<inputsticker>))
    return <- q

def from_JV ( var v:JsonValue?; t:addstickertoset? ) : addstickertoset?
    return null if v==null
    var q = new addstickertoset
    *q <- from_JV(v,type<addstickertoset>)
    return q

struct setstickerpositioninset
    //!  Use this method to move a sticker in a set created by the bot to a specific position. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    position : int	//! Required. New sticker position in the set, zero-based

def from_JV ( var v:JsonValue?; t:setstickerpositioninset ) : setstickerpositioninset
    var q : setstickerpositioninset
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<string>))
    move_to_ref(q.position, _::from_JV((v as _object)["position"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:setstickerpositioninset? ) : setstickerpositioninset?
    return null if v==null
    var q = new setstickerpositioninset
    *q <- from_JV(v,type<setstickerpositioninset>)
    return q

struct deletestickerfromset
    //!  Use this method to delete a sticker from a set created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker

def from_JV ( var v:JsonValue?; t:deletestickerfromset ) : deletestickerfromset
    var q : deletestickerfromset
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletestickerfromset? ) : deletestickerfromset?
    return null if v==null
    var q = new deletestickerfromset
    *q <- from_JV(v,type<deletestickerfromset>)
    return q

struct setstickeremojilist
    //!  Use this method to change the list of emoji assigned to a regular or custom emoji sticker. The sticker must belong to a sticker set created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    emoji_list : array<string>	//! Required. A JSON-serialized list of 1-20 emoji associated with the sticker

def from_JV ( var v:JsonValue?; t:setstickeremojilist ) : setstickeremojilist
    var q : setstickeremojilist
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<string>))
    move_to_ref(q.emoji_list, _::from_JV((v as _object)["emoji_list"], type<array<string>>))
    return <- q

def from_JV ( var v:JsonValue?; t:setstickeremojilist? ) : setstickeremojilist?
    return null if v==null
    var q = new setstickeremojilist
    *q <- from_JV(v,type<setstickeremojilist>)
    return q

struct setstickerkeywords
    //!  Use this method to change search keywords assigned to a regular or custom emoji sticker. The sticker must belong to a sticker set created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    [[optional]] keywords : array<string>	//! Optional. A JSON-serialized list of 0-20 search keywords for the sticker with total length of up to 64 characters

def from_JV ( var v:JsonValue?; t:setstickerkeywords ) : setstickerkeywords
    var q : setstickerkeywords
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<string>))
    move_to_ref(q.keywords, _::from_JV((v as _object)["keywords"], type<array<string>>))
    return <- q

def from_JV ( var v:JsonValue?; t:setstickerkeywords? ) : setstickerkeywords?
    return null if v==null
    var q = new setstickerkeywords
    *q <- from_JV(v,type<setstickerkeywords>)
    return q

struct setstickermaskposition
    //!  Use this method to change the mask position of a mask sticker. The sticker must belong to a sticker set that was created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    [[optional]] mask_position : maskposition?	//! Optional. A JSON-serialized object with the position where the mask should be placed on faces. Omit the parameter to remove the mask position.

def from_JV ( var v:JsonValue?; t:setstickermaskposition ) : setstickermaskposition
    var q : setstickermaskposition
    move_to_ref(q.sticker, _::from_JV((v as _object)["sticker"], type<string>))
    move_to_ref(q.mask_position, _::from_JV((v as _object)["mask_position"], type<maskposition?>))
    return <- q

def from_JV ( var v:JsonValue?; t:setstickermaskposition? ) : setstickermaskposition?
    return null if v==null
    var q = new setstickermaskposition
    *q <- from_JV(v,type<setstickermaskposition>)
    return q

struct setstickersettitle
    //!  Use this method to set the title of a created sticker set. Returns True on success.
    name : string	//! Required. Sticker set name
    title : string	//! Required. Sticker set title, 1-64 characters

def from_JV ( var v:JsonValue?; t:setstickersettitle ) : setstickersettitle
    var q : setstickersettitle
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setstickersettitle? ) : setstickersettitle?
    return null if v==null
    var q = new setstickersettitle
    *q <- from_JV(v,type<setstickersettitle>)
    return q

struct setstickersetthumbnail
    //!  Use this method to set the thumbnail of a regular or mask sticker set. The format of the thumbnail file must match the format of the stickers in the set. Returns True on success.
    name : string	//! Required. Sticker set name
    user_id : int	//! Required. User identifier of the sticker set owner
    [[optional]] thumbnail : inputfile?	//! Optional. A .WEBP or .PNG image with the thumbnail, must be up to 128 kilobytes in size and have a width and height of exactly 100px, or a .TGS animation with a thumbnail up to 32 kilobytes in size (see https://core.telegram.org/stickers#animated-sticker-requirements for animated sticker technical requirements), or a WEBM video with the thumbnail up to 32 kilobytes in size; see https://core.telegram.org/stickers#video-sticker-requirements for video sticker technical requirements. Pass a file_id as a String to send a file that already exists on the Telegram servers, pass an HTTP URL as a String for Telegram to get a file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files ». Animated and video sticker set thumbnails can't be uploaded via HTTP URL. If omitted, then the thumbnail is dropped and the first sticker is used as the thumbnail.

def from_JV ( var v:JsonValue?; t:setstickersetthumbnail ) : setstickersetthumbnail
    var q : setstickersetthumbnail
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.thumbnail, _::from_JV((v as _object)["thumbnail"], type<inputfile?>))
    return <- q

def from_JV ( var v:JsonValue?; t:setstickersetthumbnail? ) : setstickersetthumbnail?
    return null if v==null
    var q = new setstickersetthumbnail
    *q <- from_JV(v,type<setstickersetthumbnail>)
    return q

struct setcustomemojistickersetthumbnail
    //!  Use this method to set the thumbnail of a custom emoji sticker set. Returns True on success.
    name : string	//! Required. Sticker set name
    [[optional]] custom_emoji_id : string	//! Optional. Custom emoji identifier of a sticker from the sticker set; pass an empty string to drop the thumbnail and use the first sticker as the thumbnail.

def from_JV ( var v:JsonValue?; t:setcustomemojistickersetthumbnail ) : setcustomemojistickersetthumbnail
    var q : setcustomemojistickersetthumbnail
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.custom_emoji_id, _::from_JV((v as _object)["custom_emoji_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setcustomemojistickersetthumbnail? ) : setcustomemojistickersetthumbnail?
    return null if v==null
    var q = new setcustomemojistickersetthumbnail
    *q <- from_JV(v,type<setcustomemojistickersetthumbnail>)
    return q

struct deletestickerset
    //!  Use this method to delete a sticker set that was created by the bot. Returns True on success.
    name : string	//! Required. Sticker set name

def from_JV ( var v:JsonValue?; t:deletestickerset ) : deletestickerset
    var q : deletestickerset
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:deletestickerset? ) : deletestickerset?
    return null if v==null
    var q = new deletestickerset
    *q <- from_JV(v,type<deletestickerset>)
    return q

struct inlinequery
    //!  This object represents an incoming inline query. When the user sends an empty query, your bot could return some default or trending results.
    id : string	//! Unique identifier for this query
    from : user	//! Sender
    query : string	//! Text of the query (up to 256 characters)
    offset : string	//! Offset of the results to be returned, can be controlled by the bot
    [[optional]] chat_type : string	//! Optional. Type of the chat from which the inline query was sent. Can be either “sender” for a private chat with the inline query sender, “private”, “group”, “supergroup”, or “channel”. The chat type should be always known for requests sent from official clients and most third-party clients, unless the request was sent from a secret chat
    [[optional]] location : location?	//! Optional. Sender location, only for bots that request user location

def from_JV ( var v:JsonValue?; t:inlinequery ) : inlinequery
    var q : inlinequery
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.query, _::from_JV((v as _object)["query"], type<string>))
    move_to_ref(q.offset, _::from_JV((v as _object)["offset"], type<string>))
    move_to_ref(q.chat_type, _::from_JV((v as _object)["chat_type"], type<string>))
    move_to_ref(q.location, _::from_JV((v as _object)["location"], type<location?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequery? ) : inlinequery?
    return null if v==null
    var q = new inlinequery
    *q <- from_JV(v,type<inlinequery>)
    return q

struct answerinlinequery
    //!  Use this method to send answers to an inline query. On success, True is returned. No more than 50 results per query are allowed.
    inline_query_id : string	//! Required. Unique identifier for the answered query
    results : array<inlinequeryresult>	//! Required. A JSON-serialized array of results for the inline query
    [[optional]] cache_time : int	//! Optional. The maximum amount of time in seconds that the result of the inline query may be cached on the server. Defaults to 300.
    [[optional]] is_personal : bool	//! Optional. Pass True if results may be cached on the server side only for the user that sent the query. By default, results may be returned to any user who sends the same query
    [[optional]] next_offset : string	//! Optional. Pass the offset that a client should send in the next query with the same text to receive more results. Pass an empty string if there are no more results or if you don't support pagination. Offset length can't exceed 64 bytes.
    [[optional]] switch_pm_text : string	//! Optional. If passed, clients will display a button with specified text that switches the user to a private chat with the bot and sends the bot a start message with the parameter switch_pm_parameter
    [[optional]] switch_pm_parameter : string	//! Optional. Deep-linking parameter for the /start message sent to the bot when user presses the switch button. 1-64 characters, only A-Z, a-z, 0-9, _ and - are allowed.Example: An inline bot that sends YouTube videos can ask the user to connect the bot to their YouTube account to adapt search results accordingly. To do this, it displays a 'Connect your YouTube account' button above the results, or even before showing any. The user presses the button, switches to a private chat with the bot and, in doing so, passes a start parameter that instructs the bot to return an OAuth link. Once done, the bot can offer a switch_inline button so that the user can easily return to the chat where they wanted to use the bot's inline capabilities.

def from_JV ( var v:JsonValue?; t:answerinlinequery ) : answerinlinequery
    var q : answerinlinequery
    move_to_ref(q.inline_query_id, _::from_JV((v as _object)["inline_query_id"], type<string>))
    move_to_ref(q.results, _::from_JV((v as _object)["results"], type<array<inlinequeryresult>>))
    move_to_ref(q.cache_time, _::from_JV((v as _object)["cache_time"], type<int>))
    move_to_ref(q.is_personal, _::from_JV((v as _object)["is_personal"], type<bool>))
    move_to_ref(q.next_offset, _::from_JV((v as _object)["next_offset"], type<string>))
    move_to_ref(q.switch_pm_text, _::from_JV((v as _object)["switch_pm_text"], type<string>))
    move_to_ref(q.switch_pm_parameter, _::from_JV((v as _object)["switch_pm_parameter"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:answerinlinequery? ) : answerinlinequery?
    return null if v==null
    var q = new answerinlinequery
    *q <- from_JV(v,type<answerinlinequery>)
    return q

struct inlinequeryresult
    //!  This object represents one result of an inline query. Telegram clients currently support results of the following 20 types: Note: All URLs passed in inline query results will be available to end users and therefore must be assumed to be public.
    object_inlinequeryresult_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:inlinequeryresult ) : inlinequeryresult
    var q : inlinequeryresult
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresult? ) : inlinequeryresult?
    return null if v==null
    var q = new inlinequeryresult
    *q <- from_JV(v,type<inlinequeryresult>)
    return q

struct inlinequeryresultarticle
    //!  Represents a link to an article or web page.
    _type : string	//! Type of the result, must be article
    id : string	//! Unique identifier for this result, 1-64 Bytes
    title : string	//! Title of the result
    input_message_content : inputmessagecontent	//! Content of the message to be sent
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] url : string	//! Optional. URL of the result
    [[optional]] hide_url : bool	//! Optional. Pass True if you don't want the URL to be shown in the message
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] thumbnail_url : string	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int	//! Optional. Thumbnail height

def from_JV ( var v:JsonValue?; t:inlinequeryresultarticle ) : inlinequeryresultarticle
    var q : inlinequeryresultarticle
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.url, _::from_JV((v as _object)["url"], type<string>))
    move_to_ref(q.hide_url, _::from_JV((v as _object)["hide_url"], type<bool>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_width, _::from_JV((v as _object)["thumbnail_width"], type<int>))
    move_to_ref(q.thumbnail_height, _::from_JV((v as _object)["thumbnail_height"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultarticle? ) : inlinequeryresultarticle?
    return null if v==null
    var q = new inlinequeryresultarticle
    *q <- from_JV(v,type<inlinequeryresultarticle>)
    return q

struct inlinequeryresultphoto
    //!  Represents a link to a photo. By default, this photo will be sent by the user with optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the photo.
    _type : string	//! Type of the result, must be photo
    id : string	//! Unique identifier for this result, 1-64 bytes
    photo_url : string	//! A valid URL of the photo. Photo must be in JPEG format. Photo size must not exceed 5MB
    thumbnail_url : string	//! URL of the thumbnail for the photo
    [[optional]] photo_width : int	//! Optional. Width of the photo
    [[optional]] photo_height : int	//! Optional. Height of the photo
    [[optional]] title : string	//! Optional. Title for the result
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] caption : string	//! Optional. Caption of the photo to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the photo

def from_JV ( var v:JsonValue?; t:inlinequeryresultphoto ) : inlinequeryresultphoto
    var q : inlinequeryresultphoto
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.photo_url, _::from_JV((v as _object)["photo_url"], type<string>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.photo_width, _::from_JV((v as _object)["photo_width"], type<int>))
    move_to_ref(q.photo_height, _::from_JV((v as _object)["photo_height"], type<int>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultphoto? ) : inlinequeryresultphoto?
    return null if v==null
    var q = new inlinequeryresultphoto
    *q <- from_JV(v,type<inlinequeryresultphoto>)
    return q

struct inlinequeryresultgif
    //!  Represents a link to an animated GIF file. By default, this animated GIF file will be sent by the user with optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the animation.
    _type : string	//! Type of the result, must be gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    gif_url : string	//! A valid URL for the GIF file. File size must not exceed 1MB
    [[optional]] gif_width : int	//! Optional. Width of the GIF
    [[optional]] gif_height : int	//! Optional. Height of the GIF
    [[optional]] gif_duration : int	//! Optional. Duration of the GIF in seconds
    thumbnail_url : string	//! URL of the static (JPEG or GIF) or animated (MPEG4) thumbnail for the result
    [[optional]] thumbnail_mime_type : string	//! Optional. MIME type of the thumbnail, must be one of “image/jpeg”, “image/gif”, or “video/mp4”. Defaults to “image/jpeg”
    [[optional]] title : string	//! Optional. Title for the result
    [[optional]] caption : string	//! Optional. Caption of the GIF file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the GIF animation

def from_JV ( var v:JsonValue?; t:inlinequeryresultgif ) : inlinequeryresultgif
    var q : inlinequeryresultgif
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.gif_url, _::from_JV((v as _object)["gif_url"], type<string>))
    move_to_ref(q.gif_width, _::from_JV((v as _object)["gif_width"], type<int>))
    move_to_ref(q.gif_height, _::from_JV((v as _object)["gif_height"], type<int>))
    move_to_ref(q.gif_duration, _::from_JV((v as _object)["gif_duration"], type<int>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_mime_type, _::from_JV((v as _object)["thumbnail_mime_type"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultgif? ) : inlinequeryresultgif?
    return null if v==null
    var q = new inlinequeryresultgif
    *q <- from_JV(v,type<inlinequeryresultgif>)
    return q

struct inlinequeryresultmpeg4gif
    //!  Represents a link to a video animation (H.264/MPEG-4 AVC video without sound). By default, this animated MPEG-4 file will be sent by the user with optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the animation.
    _type : string	//! Type of the result, must be mpeg4_gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    mpeg4_url : string	//! A valid URL for the MPEG4 file. File size must not exceed 1MB
    [[optional]] mpeg4_width : int	//! Optional. Video width
    [[optional]] mpeg4_height : int	//! Optional. Video height
    [[optional]] mpeg4_duration : int	//! Optional. Video duration in seconds
    thumbnail_url : string	//! URL of the static (JPEG or GIF) or animated (MPEG4) thumbnail for the result
    [[optional]] thumbnail_mime_type : string	//! Optional. MIME type of the thumbnail, must be one of “image/jpeg”, “image/gif”, or “video/mp4”. Defaults to “image/jpeg”
    [[optional]] title : string	//! Optional. Title for the result
    [[optional]] caption : string	//! Optional. Caption of the MPEG-4 file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video animation

def from_JV ( var v:JsonValue?; t:inlinequeryresultmpeg4gif ) : inlinequeryresultmpeg4gif
    var q : inlinequeryresultmpeg4gif
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.mpeg4_url, _::from_JV((v as _object)["mpeg4_url"], type<string>))
    move_to_ref(q.mpeg4_width, _::from_JV((v as _object)["mpeg4_width"], type<int>))
    move_to_ref(q.mpeg4_height, _::from_JV((v as _object)["mpeg4_height"], type<int>))
    move_to_ref(q.mpeg4_duration, _::from_JV((v as _object)["mpeg4_duration"], type<int>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_mime_type, _::from_JV((v as _object)["thumbnail_mime_type"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultmpeg4gif? ) : inlinequeryresultmpeg4gif?
    return null if v==null
    var q = new inlinequeryresultmpeg4gif
    *q <- from_JV(v,type<inlinequeryresultmpeg4gif>)
    return q

struct inlinequeryresultvideo
    //!  Represents a link to a page containing an embedded video player or a video file. By default, this video file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the video.
    _type : string	//! Type of the result, must be video
    id : string	//! Unique identifier for this result, 1-64 bytes
    video_url : string	//! A valid URL for the embedded video player or video file
    mime_type : string	//! MIME type of the content of the video URL, “text/html” or “video/mp4”
    thumbnail_url : string	//! URL of the thumbnail (JPEG only) for the video
    title : string	//! Title for the result
    [[optional]] caption : string	//! Optional. Caption of the video to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] video_width : int	//! Optional. Video width
    [[optional]] video_height : int	//! Optional. Video height
    [[optional]] video_duration : int	//! Optional. Video duration in seconds
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video. This field is required if InlineQueryResultVideo is used to send an HTML-page as a result (e.g., a YouTube video).

def from_JV ( var v:JsonValue?; t:inlinequeryresultvideo ) : inlinequeryresultvideo
    var q : inlinequeryresultvideo
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.video_url, _::from_JV((v as _object)["video_url"], type<string>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.video_width, _::from_JV((v as _object)["video_width"], type<int>))
    move_to_ref(q.video_height, _::from_JV((v as _object)["video_height"], type<int>))
    move_to_ref(q.video_duration, _::from_JV((v as _object)["video_duration"], type<int>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultvideo? ) : inlinequeryresultvideo?
    return null if v==null
    var q = new inlinequeryresultvideo
    *q <- from_JV(v,type<inlinequeryresultvideo>)
    return q

struct inlinequeryresultaudio
    //!  Represents a link to an MP3 audio file. By default, this audio file will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the audio.
    _type : string	//! Type of the result, must be audio
    id : string	//! Unique identifier for this result, 1-64 bytes
    audio_url : string	//! A valid URL for the audio file
    title : string	//! Title
    [[optional]] caption : string	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] performer : string	//! Optional. Performer
    [[optional]] audio_duration : int	//! Optional. Audio duration in seconds
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the audio

def from_JV ( var v:JsonValue?; t:inlinequeryresultaudio ) : inlinequeryresultaudio
    var q : inlinequeryresultaudio
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.audio_url, _::from_JV((v as _object)["audio_url"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.performer, _::from_JV((v as _object)["performer"], type<string>))
    move_to_ref(q.audio_duration, _::from_JV((v as _object)["audio_duration"], type<int>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultaudio? ) : inlinequeryresultaudio?
    return null if v==null
    var q = new inlinequeryresultaudio
    *q <- from_JV(v,type<inlinequeryresultaudio>)
    return q

struct inlinequeryresultvoice
    //!  Represents a link to a voice recording in an .OGG container encoded with OPUS. By default, this voice recording will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the the voice message.
    _type : string	//! Type of the result, must be voice
    id : string	//! Unique identifier for this result, 1-64 bytes
    voice_url : string	//! A valid URL for the voice recording
    title : string	//! Recording title
    [[optional]] caption : string	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] voice_duration : int	//! Optional. Recording duration in seconds
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the voice recording

def from_JV ( var v:JsonValue?; t:inlinequeryresultvoice ) : inlinequeryresultvoice
    var q : inlinequeryresultvoice
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.voice_url, _::from_JV((v as _object)["voice_url"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.voice_duration, _::from_JV((v as _object)["voice_duration"], type<int>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultvoice? ) : inlinequeryresultvoice?
    return null if v==null
    var q = new inlinequeryresultvoice
    *q <- from_JV(v,type<inlinequeryresultvoice>)
    return q

struct inlinequeryresultdocument
    //!  Represents a link to a file. By default, this file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the file. Currently, only .PDF and .ZIP files can be sent using this method.
    _type : string	//! Type of the result, must be document
    id : string	//! Unique identifier for this result, 1-64 bytes
    title : string	//! Title for the result
    [[optional]] caption : string	//! Optional. Caption of the document to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    document_url : string	//! A valid URL for the file
    mime_type : string	//! MIME type of the content of the file, either “application/pdf” or “application/zip”
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the file
    [[optional]] thumbnail_url : string	//! Optional. URL of the thumbnail (JPEG only) for the file
    [[optional]] thumbnail_width : int	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int	//! Optional. Thumbnail height

def from_JV ( var v:JsonValue?; t:inlinequeryresultdocument ) : inlinequeryresultdocument
    var q : inlinequeryresultdocument
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.document_url, _::from_JV((v as _object)["document_url"], type<string>))
    move_to_ref(q.mime_type, _::from_JV((v as _object)["mime_type"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_width, _::from_JV((v as _object)["thumbnail_width"], type<int>))
    move_to_ref(q.thumbnail_height, _::from_JV((v as _object)["thumbnail_height"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultdocument? ) : inlinequeryresultdocument?
    return null if v==null
    var q = new inlinequeryresultdocument
    *q <- from_JV(v,type<inlinequeryresultdocument>)
    return q

struct inlinequeryresultlocation
    //!  Represents a location on a map. By default, the location will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the location.
    _type : string	//! Type of the result, must be location
    id : string	//! Unique identifier for this result, 1-64 Bytes
    latitude : float	//! Location latitude in degrees
    longitude : float	//! Location longitude in degrees
    title : string	//! Location title
    [[optional]] horizontal_accuracy : float	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int	//! Optional. Period in seconds for which the location can be updated, should be between 60 and 86400.
    [[optional]] heading : int	//! Optional. For live locations, a direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int	//! Optional. For live locations, a maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the location
    [[optional]] thumbnail_url : string	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int	//! Optional. Thumbnail height

def from_JV ( var v:JsonValue?; t:inlinequeryresultlocation ) : inlinequeryresultlocation
    var q : inlinequeryresultlocation
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.horizontal_accuracy, _::from_JV((v as _object)["horizontal_accuracy"], type<float>))
    move_to_ref(q.live_period, _::from_JV((v as _object)["live_period"], type<int>))
    move_to_ref(q.heading, _::from_JV((v as _object)["heading"], type<int>))
    move_to_ref(q.proximity_alert_radius, _::from_JV((v as _object)["proximity_alert_radius"], type<int>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_width, _::from_JV((v as _object)["thumbnail_width"], type<int>))
    move_to_ref(q.thumbnail_height, _::from_JV((v as _object)["thumbnail_height"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultlocation? ) : inlinequeryresultlocation?
    return null if v==null
    var q = new inlinequeryresultlocation
    *q <- from_JV(v,type<inlinequeryresultlocation>)
    return q

struct inlinequeryresultvenue
    //!  Represents a venue. By default, the venue will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the venue.
    _type : string	//! Type of the result, must be venue
    id : string	//! Unique identifier for this result, 1-64 Bytes
    latitude : float	//! Latitude of the venue location in degrees
    longitude : float	//! Longitude of the venue location in degrees
    title : string	//! Title of the venue
    address : string	//! Address of the venue
    [[optional]] foursquare_id : string	//! Optional. Foursquare identifier of the venue if known
    [[optional]] foursquare_type : string	//! Optional. Foursquare type of the venue, if known. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string	//! Optional. Google Places type of the venue. (See supported types.)
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the venue
    [[optional]] thumbnail_url : string	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int	//! Optional. Thumbnail height

def from_JV ( var v:JsonValue?; t:inlinequeryresultvenue ) : inlinequeryresultvenue
    var q : inlinequeryresultvenue
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.address, _::from_JV((v as _object)["address"], type<string>))
    move_to_ref(q.foursquare_id, _::from_JV((v as _object)["foursquare_id"], type<string>))
    move_to_ref(q.foursquare_type, _::from_JV((v as _object)["foursquare_type"], type<string>))
    move_to_ref(q.google_place_id, _::from_JV((v as _object)["google_place_id"], type<string>))
    move_to_ref(q.google_place_type, _::from_JV((v as _object)["google_place_type"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_width, _::from_JV((v as _object)["thumbnail_width"], type<int>))
    move_to_ref(q.thumbnail_height, _::from_JV((v as _object)["thumbnail_height"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultvenue? ) : inlinequeryresultvenue?
    return null if v==null
    var q = new inlinequeryresultvenue
    *q <- from_JV(v,type<inlinequeryresultvenue>)
    return q

struct inlinequeryresultcontact
    //!  Represents a contact with a phone number. By default, this contact will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the contact.
    _type : string	//! Type of the result, must be contact
    id : string	//! Unique identifier for this result, 1-64 Bytes
    phone_number : string	//! Contact's phone number
    first_name : string	//! Contact's first name
    [[optional]] last_name : string	//! Optional. Contact's last name
    [[optional]] vcard : string	//! Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the contact
    [[optional]] thumbnail_url : string	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int	//! Optional. Thumbnail height

def from_JV ( var v:JsonValue?; t:inlinequeryresultcontact ) : inlinequeryresultcontact
    var q : inlinequeryresultcontact
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.phone_number, _::from_JV((v as _object)["phone_number"], type<string>))
    move_to_ref(q.first_name, _::from_JV((v as _object)["first_name"], type<string>))
    move_to_ref(q.last_name, _::from_JV((v as _object)["last_name"], type<string>))
    move_to_ref(q.vcard, _::from_JV((v as _object)["vcard"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    move_to_ref(q.thumbnail_url, _::from_JV((v as _object)["thumbnail_url"], type<string>))
    move_to_ref(q.thumbnail_width, _::from_JV((v as _object)["thumbnail_width"], type<int>))
    move_to_ref(q.thumbnail_height, _::from_JV((v as _object)["thumbnail_height"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcontact? ) : inlinequeryresultcontact?
    return null if v==null
    var q = new inlinequeryresultcontact
    *q <- from_JV(v,type<inlinequeryresultcontact>)
    return q

struct inlinequeryresultgame
    //!  Represents a Game.
    _type : string	//! Type of the result, must be game
    id : string	//! Unique identifier for this result, 1-64 bytes
    game_short_name : string	//! Short name of the game
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message

def from_JV ( var v:JsonValue?; t:inlinequeryresultgame ) : inlinequeryresultgame
    var q : inlinequeryresultgame
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.game_short_name, _::from_JV((v as _object)["game_short_name"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultgame? ) : inlinequeryresultgame?
    return null if v==null
    var q = new inlinequeryresultgame
    *q <- from_JV(v,type<inlinequeryresultgame>)
    return q

struct inlinequeryresultcachedphoto
    //!  Represents a link to a photo stored on the Telegram servers. By default, this photo will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the photo.
    _type : string	//! Type of the result, must be photo
    id : string	//! Unique identifier for this result, 1-64 bytes
    photo_file_id : string	//! A valid file identifier of the photo
    [[optional]] title : string	//! Optional. Title for the result
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] caption : string	//! Optional. Caption of the photo to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the photo

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedphoto ) : inlinequeryresultcachedphoto
    var q : inlinequeryresultcachedphoto
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.photo_file_id, _::from_JV((v as _object)["photo_file_id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedphoto? ) : inlinequeryresultcachedphoto?
    return null if v==null
    var q = new inlinequeryresultcachedphoto
    *q <- from_JV(v,type<inlinequeryresultcachedphoto>)
    return q

struct inlinequeryresultcachedgif
    //!  Represents a link to an animated GIF file stored on the Telegram servers. By default, this animated GIF file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with specified content instead of the animation.
    _type : string	//! Type of the result, must be gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    gif_file_id : string	//! A valid file identifier for the GIF file
    [[optional]] title : string	//! Optional. Title for the result
    [[optional]] caption : string	//! Optional. Caption of the GIF file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the GIF animation

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedgif ) : inlinequeryresultcachedgif
    var q : inlinequeryresultcachedgif
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.gif_file_id, _::from_JV((v as _object)["gif_file_id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedgif? ) : inlinequeryresultcachedgif?
    return null if v==null
    var q = new inlinequeryresultcachedgif
    *q <- from_JV(v,type<inlinequeryresultcachedgif>)
    return q

struct inlinequeryresultcachedmpeg4gif
    //!  Represents a link to a video animation (H.264/MPEG-4 AVC video without sound) stored on the Telegram servers. By default, this animated MPEG-4 file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the animation.
    _type : string	//! Type of the result, must be mpeg4_gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    mpeg4_file_id : string	//! A valid file identifier for the MPEG4 file
    [[optional]] title : string	//! Optional. Title for the result
    [[optional]] caption : string	//! Optional. Caption of the MPEG-4 file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video animation

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedmpeg4gif ) : inlinequeryresultcachedmpeg4gif
    var q : inlinequeryresultcachedmpeg4gif
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.mpeg4_file_id, _::from_JV((v as _object)["mpeg4_file_id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedmpeg4gif? ) : inlinequeryresultcachedmpeg4gif?
    return null if v==null
    var q = new inlinequeryresultcachedmpeg4gif
    *q <- from_JV(v,type<inlinequeryresultcachedmpeg4gif>)
    return q

struct inlinequeryresultcachedsticker
    //!  Represents a link to a sticker stored on the Telegram servers. By default, this sticker will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the sticker.
    _type : string	//! Type of the result, must be sticker
    id : string	//! Unique identifier for this result, 1-64 bytes
    sticker_file_id : string	//! A valid file identifier of the sticker
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the sticker

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedsticker ) : inlinequeryresultcachedsticker
    var q : inlinequeryresultcachedsticker
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.sticker_file_id, _::from_JV((v as _object)["sticker_file_id"], type<string>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedsticker? ) : inlinequeryresultcachedsticker?
    return null if v==null
    var q = new inlinequeryresultcachedsticker
    *q <- from_JV(v,type<inlinequeryresultcachedsticker>)
    return q

struct inlinequeryresultcacheddocument
    //!  Represents a link to a file stored on the Telegram servers. By default, this file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the file.
    _type : string	//! Type of the result, must be document
    id : string	//! Unique identifier for this result, 1-64 bytes
    title : string	//! Title for the result
    document_file_id : string	//! A valid file identifier for the file
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] caption : string	//! Optional. Caption of the document to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the file

def from_JV ( var v:JsonValue?; t:inlinequeryresultcacheddocument ) : inlinequeryresultcacheddocument
    var q : inlinequeryresultcacheddocument
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.document_file_id, _::from_JV((v as _object)["document_file_id"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcacheddocument? ) : inlinequeryresultcacheddocument?
    return null if v==null
    var q = new inlinequeryresultcacheddocument
    *q <- from_JV(v,type<inlinequeryresultcacheddocument>)
    return q

struct inlinequeryresultcachedvideo
    //!  Represents a link to a video file stored on the Telegram servers. By default, this video file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the video.
    _type : string	//! Type of the result, must be video
    id : string	//! Unique identifier for this result, 1-64 bytes
    video_file_id : string	//! A valid file identifier for the video file
    title : string	//! Title for the result
    [[optional]] description : string	//! Optional. Short description of the result
    [[optional]] caption : string	//! Optional. Caption of the video to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedvideo ) : inlinequeryresultcachedvideo
    var q : inlinequeryresultcachedvideo
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.video_file_id, _::from_JV((v as _object)["video_file_id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedvideo? ) : inlinequeryresultcachedvideo?
    return null if v==null
    var q = new inlinequeryresultcachedvideo
    *q <- from_JV(v,type<inlinequeryresultcachedvideo>)
    return q

struct inlinequeryresultcachedvoice
    //!  Represents a link to a voice message stored on the Telegram servers. By default, this voice message will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the voice message.
    _type : string	//! Type of the result, must be voice
    id : string	//! Unique identifier for this result, 1-64 bytes
    voice_file_id : string	//! A valid file identifier for the voice message
    title : string	//! Voice message title
    [[optional]] caption : string	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the voice message

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedvoice ) : inlinequeryresultcachedvoice
    var q : inlinequeryresultcachedvoice
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.voice_file_id, _::from_JV((v as _object)["voice_file_id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedvoice? ) : inlinequeryresultcachedvoice?
    return null if v==null
    var q = new inlinequeryresultcachedvoice
    *q <- from_JV(v,type<inlinequeryresultcachedvoice>)
    return q

struct inlinequeryresultcachedaudio
    //!  Represents a link to an MP3 audio file stored on the Telegram servers. By default, this audio file will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the audio.
    _type : string	//! Type of the result, must be audio
    id : string	//! Unique identifier for this result, 1-64 bytes
    audio_file_id : string	//! A valid file identifier for the audio file
    [[optional]] caption : string	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the audio

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedaudio ) : inlinequeryresultcachedaudio
    var q : inlinequeryresultcachedaudio
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.audio_file_id, _::from_JV((v as _object)["audio_file_id"], type<string>))
    move_to_ref(q.caption, _::from_JV((v as _object)["caption"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.caption_entities, _::from_JV((v as _object)["caption_entities"], type<array<messageentity>>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    move_to_ref(q.input_message_content, _::from_JV((v as _object)["input_message_content"], type<inputmessagecontent?>))
    return <- q

def from_JV ( var v:JsonValue?; t:inlinequeryresultcachedaudio? ) : inlinequeryresultcachedaudio?
    return null if v==null
    var q = new inlinequeryresultcachedaudio
    *q <- from_JV(v,type<inlinequeryresultcachedaudio>)
    return q

struct inputmessagecontent
    //!  This object represents the content of a message to be sent as a result of an inline query. Telegram clients currently support the following 5 types:
    object_inputmessagecontent_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:inputmessagecontent ) : inputmessagecontent
    var q : inputmessagecontent
    return <- q

def from_JV ( var v:JsonValue?; t:inputmessagecontent? ) : inputmessagecontent?
    return null if v==null
    var q = new inputmessagecontent
    *q <- from_JV(v,type<inputmessagecontent>)
    return q

struct inputtextmessagecontent
    //!  Represents the content of a text message to be sent as the result of an inline query.
    message_text : string	//! Text of the message to be sent, 1-4096 characters
    [[optional]] parse_mode : string	//! Optional. Mode for parsing entities in the message text. See formatting options for more details.
    [[optional]] entities : array<messageentity>	//! Optional. List of special entities that appear in message text, which can be specified instead of parse_mode
    [[optional]] disable_web_page_preview : bool	//! Optional. Disables link previews for links in the sent message

def from_JV ( var v:JsonValue?; t:inputtextmessagecontent ) : inputtextmessagecontent
    var q : inputtextmessagecontent
    move_to_ref(q.message_text, _::from_JV((v as _object)["message_text"], type<string>))
    move_to_ref(q.parse_mode, _::from_JV((v as _object)["parse_mode"], type<string>))
    move_to_ref(q.entities, _::from_JV((v as _object)["entities"], type<array<messageentity>>))
    move_to_ref(q.disable_web_page_preview, _::from_JV((v as _object)["disable_web_page_preview"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputtextmessagecontent? ) : inputtextmessagecontent?
    return null if v==null
    var q = new inputtextmessagecontent
    *q <- from_JV(v,type<inputtextmessagecontent>)
    return q

struct inputlocationmessagecontent
    //!  Represents the content of a location message to be sent as the result of an inline query.
    latitude : float	//! Latitude of the location in degrees
    longitude : float	//! Longitude of the location in degrees
    [[optional]] horizontal_accuracy : float	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int	//! Optional. Period in seconds for which the location can be updated, should be between 60 and 86400.
    [[optional]] heading : int	//! Optional. For live locations, a direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int	//! Optional. For live locations, a maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.

def from_JV ( var v:JsonValue?; t:inputlocationmessagecontent ) : inputlocationmessagecontent
    var q : inputlocationmessagecontent
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.horizontal_accuracy, _::from_JV((v as _object)["horizontal_accuracy"], type<float>))
    move_to_ref(q.live_period, _::from_JV((v as _object)["live_period"], type<int>))
    move_to_ref(q.heading, _::from_JV((v as _object)["heading"], type<int>))
    move_to_ref(q.proximity_alert_radius, _::from_JV((v as _object)["proximity_alert_radius"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputlocationmessagecontent? ) : inputlocationmessagecontent?
    return null if v==null
    var q = new inputlocationmessagecontent
    *q <- from_JV(v,type<inputlocationmessagecontent>)
    return q

struct inputvenuemessagecontent
    //!  Represents the content of a venue message to be sent as the result of an inline query.
    latitude : float	//! Latitude of the venue in degrees
    longitude : float	//! Longitude of the venue in degrees
    title : string	//! Name of the venue
    address : string	//! Address of the venue
    [[optional]] foursquare_id : string	//! Optional. Foursquare identifier of the venue, if known
    [[optional]] foursquare_type : string	//! Optional. Foursquare type of the venue, if known. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string	//! Optional. Google Places type of the venue. (See supported types.)

def from_JV ( var v:JsonValue?; t:inputvenuemessagecontent ) : inputvenuemessagecontent
    var q : inputvenuemessagecontent
    move_to_ref(q.latitude, _::from_JV((v as _object)["latitude"], type<float>))
    move_to_ref(q.longitude, _::from_JV((v as _object)["longitude"], type<float>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.address, _::from_JV((v as _object)["address"], type<string>))
    move_to_ref(q.foursquare_id, _::from_JV((v as _object)["foursquare_id"], type<string>))
    move_to_ref(q.foursquare_type, _::from_JV((v as _object)["foursquare_type"], type<string>))
    move_to_ref(q.google_place_id, _::from_JV((v as _object)["google_place_id"], type<string>))
    move_to_ref(q.google_place_type, _::from_JV((v as _object)["google_place_type"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputvenuemessagecontent? ) : inputvenuemessagecontent?
    return null if v==null
    var q = new inputvenuemessagecontent
    *q <- from_JV(v,type<inputvenuemessagecontent>)
    return q

struct inputcontactmessagecontent
    //!  Represents the content of a contact message to be sent as the result of an inline query.
    phone_number : string	//! Contact's phone number
    first_name : string	//! Contact's first name
    [[optional]] last_name : string	//! Optional. Contact's last name
    [[optional]] vcard : string	//! Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes

def from_JV ( var v:JsonValue?; t:inputcontactmessagecontent ) : inputcontactmessagecontent
    var q : inputcontactmessagecontent
    move_to_ref(q.phone_number, _::from_JV((v as _object)["phone_number"], type<string>))
    move_to_ref(q.first_name, _::from_JV((v as _object)["first_name"], type<string>))
    move_to_ref(q.last_name, _::from_JV((v as _object)["last_name"], type<string>))
    move_to_ref(q.vcard, _::from_JV((v as _object)["vcard"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputcontactmessagecontent? ) : inputcontactmessagecontent?
    return null if v==null
    var q = new inputcontactmessagecontent
    *q <- from_JV(v,type<inputcontactmessagecontent>)
    return q

struct inputinvoicemessagecontent
    //!  Represents the content of an invoice message to be sent as the result of an inline query.
    title : string	//! Product name, 1-32 characters
    description : string	//! Product description, 1-255 characters
    payload : string	//! Bot-defined invoice payload, 1-128 bytes. This will not be displayed to the user, use for your internal processes.
    provider_token : string	//! Payment provider token, obtained via @BotFather
    currency : string	//! Three-letter ISO 4217 currency code, see more on currencies
    prices : array<labeledprice>	//! Price breakdown, a JSON-serialized list of components (e.g. product price, tax, discount, delivery cost, delivery tax, bonus, etc.)
    [[optional]] max_tip_amount : int	//! Optional. The maximum accepted amount for tips in the smallest units of the currency (integer, not float/double). For example, for a maximum tip of US$ 1.45 pass max_tip_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies). Defaults to 0
    [[optional]] suggested_tip_amounts : array<int>	//! Optional. A JSON-serialized array of suggested amounts of tip in the smallest units of the currency (integer, not float/double). At most 4 suggested tip amounts can be specified. The suggested tip amounts must be positive, passed in a strictly increased order and must not exceed max_tip_amount.
    [[optional]] provider_data : string	//! Optional. A JSON-serialized object for data about the invoice, which will be shared with the payment provider. A detailed description of the required fields should be provided by the payment provider.
    [[optional]] photo_url : string	//! Optional. URL of the product photo for the invoice. Can be a photo of the goods or a marketing image for a service.
    [[optional]] photo_size : int	//! Optional. Photo size in bytes
    [[optional]] photo_width : int	//! Optional. Photo width
    [[optional]] photo_height : int	//! Optional. Photo height
    [[optional]] need_name : bool	//! Optional. Pass True if you require the user's full name to complete the order
    [[optional]] need_phone_number : bool	//! Optional. Pass True if you require the user's phone number to complete the order
    [[optional]] need_email : bool	//! Optional. Pass True if you require the user's email address to complete the order
    [[optional]] need_shipping_address : bool	//! Optional. Pass True if you require the user's shipping address to complete the order
    [[optional]] send_phone_number_to_provider : bool	//! Optional. Pass True if the user's phone number should be sent to provider
    [[optional]] send_email_to_provider : bool	//! Optional. Pass True if the user's email address should be sent to provider
    [[optional]] is_flexible : bool	//! Optional. Pass True if the final price depends on the shipping method

def from_JV ( var v:JsonValue?; t:inputinvoicemessagecontent ) : inputinvoicemessagecontent
    var q : inputinvoicemessagecontent
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.payload, _::from_JV((v as _object)["payload"], type<string>))
    move_to_ref(q.provider_token, _::from_JV((v as _object)["provider_token"], type<string>))
    move_to_ref(q.currency, _::from_JV((v as _object)["currency"], type<string>))
    move_to_ref(q.prices, _::from_JV((v as _object)["prices"], type<array<labeledprice>>))
    move_to_ref(q.max_tip_amount, _::from_JV((v as _object)["max_tip_amount"], type<int>))
    move_to_ref(q.suggested_tip_amounts, _::from_JV((v as _object)["suggested_tip_amounts"], type<array<int>>))
    move_to_ref(q.provider_data, _::from_JV((v as _object)["provider_data"], type<string>))
    move_to_ref(q.photo_url, _::from_JV((v as _object)["photo_url"], type<string>))
    move_to_ref(q.photo_size, _::from_JV((v as _object)["photo_size"], type<int>))
    move_to_ref(q.photo_width, _::from_JV((v as _object)["photo_width"], type<int>))
    move_to_ref(q.photo_height, _::from_JV((v as _object)["photo_height"], type<int>))
    move_to_ref(q.need_name, _::from_JV((v as _object)["need_name"], type<bool>))
    move_to_ref(q.need_phone_number, _::from_JV((v as _object)["need_phone_number"], type<bool>))
    move_to_ref(q.need_email, _::from_JV((v as _object)["need_email"], type<bool>))
    move_to_ref(q.need_shipping_address, _::from_JV((v as _object)["need_shipping_address"], type<bool>))
    move_to_ref(q.send_phone_number_to_provider, _::from_JV((v as _object)["send_phone_number_to_provider"], type<bool>))
    move_to_ref(q.send_email_to_provider, _::from_JV((v as _object)["send_email_to_provider"], type<bool>))
    move_to_ref(q.is_flexible, _::from_JV((v as _object)["is_flexible"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:inputinvoicemessagecontent? ) : inputinvoicemessagecontent?
    return null if v==null
    var q = new inputinvoicemessagecontent
    *q <- from_JV(v,type<inputinvoicemessagecontent>)
    return q

struct choseninlineresult
    //!  Represents a result of an inline query that was chosen by the user and sent to their chat partner.
    result_id : string	//! The unique identifier for the result that was chosen
    from : user	//! The user that chose the result
    [[optional]] location : location?	//! Optional. Sender location, only for bots that require user location
    [[optional]] inline_message_id : string	//! Optional. Identifier of the sent inline message. Available only if there is an inline keyboard attached to the message. Will be also received in callback queries and can be used to edit the message.
    query : string	//! The query that was used to obtain the result

def from_JV ( var v:JsonValue?; t:choseninlineresult ) : choseninlineresult
    var q : choseninlineresult
    move_to_ref(q.result_id, _::from_JV((v as _object)["result_id"], type<string>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.location, _::from_JV((v as _object)["location"], type<location?>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    move_to_ref(q.query, _::from_JV((v as _object)["query"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:choseninlineresult? ) : choseninlineresult?
    return null if v==null
    var q = new choseninlineresult
    *q <- from_JV(v,type<choseninlineresult>)
    return q

struct answerwebappquery
    //!  Use this method to set the result of an interaction with a Web App and send a corresponding message on behalf of the user to the chat from which the query originated. On success, a SentWebAppMessage object is returned.
    web_app_query_id : string	//! Required. Unique identifier for the query to be answered
    result : inlinequeryresult	//! Required. A JSON-serialized object describing the message to be sent

def from_JV ( var v:JsonValue?; t:answerwebappquery ) : answerwebappquery
    var q : answerwebappquery
    move_to_ref(q.web_app_query_id, _::from_JV((v as _object)["web_app_query_id"], type<string>))
    move_to_ref(q.result, _::from_JV((v as _object)["result"], type<inlinequeryresult>))
    return <- q

def from_JV ( var v:JsonValue?; t:answerwebappquery? ) : answerwebappquery?
    return null if v==null
    var q = new answerwebappquery
    *q <- from_JV(v,type<answerwebappquery>)
    return q

struct sentwebappmessage
    //!  Describes an inline message sent by a Web App on behalf of a user.
    [[optional]] inline_message_id : string	//! Optional. Identifier of the sent inline message. Available only if there is an inline keyboard attached to the message.

def from_JV ( var v:JsonValue?; t:sentwebappmessage ) : sentwebappmessage
    var q : sentwebappmessage
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:sentwebappmessage? ) : sentwebappmessage?
    return null if v==null
    var q = new sentwebappmessage
    *q <- from_JV(v,type<sentwebappmessage>)
    return q

struct sendinvoice
    //!  Use this method to send invoices. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    title : string	//! Required. Product name, 1-32 characters
    description : string	//! Required. Product description, 1-255 characters
    payload : string	//! Required. Bot-defined invoice payload, 1-128 bytes. This will not be displayed to the user, use for your internal processes.
    provider_token : string	//! Required. Payment provider token, obtained via @BotFather
    currency : string	//! Required. Three-letter ISO 4217 currency code, see more on currencies
    prices : array<labeledprice>	//! Required. Price breakdown, a JSON-serialized list of components (e.g. product price, tax, discount, delivery cost, delivery tax, bonus, etc.)
    [[optional]] max_tip_amount : int	//! Optional. The maximum accepted amount for tips in the smallest units of the currency (integer, not float/double). For example, for a maximum tip of US$ 1.45 pass max_tip_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies). Defaults to 0
    [[optional]] suggested_tip_amounts : array<int>	//! Optional. A JSON-serialized array of suggested amounts of tips in the smallest units of the currency (integer, not float/double). At most 4 suggested tip amounts can be specified. The suggested tip amounts must be positive, passed in a strictly increased order and must not exceed max_tip_amount.
    [[optional]] start_parameter : string	//! Optional. Unique deep-linking parameter. If left empty, forwarded copies of the sent message will have a Pay button, allowing multiple users to pay directly from the forwarded message, using the same invoice. If non-empty, forwarded copies of the sent message will have a URL button with a deep link to the bot (instead of a Pay button), with the value used as the start parameter
    [[optional]] provider_data : string	//! Optional. JSON-serialized data about the invoice, which will be shared with the payment provider. A detailed description of required fields should be provided by the payment provider.
    [[optional]] photo_url : string	//! Optional. URL of the product photo for the invoice. Can be a photo of the goods or a marketing image for a service. People like it better when they see what they are paying for.
    [[optional]] photo_size : int	//! Optional. Photo size in bytes
    [[optional]] photo_width : int	//! Optional. Photo width
    [[optional]] photo_height : int	//! Optional. Photo height
    [[optional]] need_name : bool	//! Optional. Pass True if you require the user's full name to complete the order
    [[optional]] need_phone_number : bool	//! Optional. Pass True if you require the user's phone number to complete the order
    [[optional]] need_email : bool	//! Optional. Pass True if you require the user's email address to complete the order
    [[optional]] need_shipping_address : bool	//! Optional. Pass True if you require the user's shipping address to complete the order
    [[optional]] send_phone_number_to_provider : bool	//! Optional. Pass True if the user's phone number should be sent to provider
    [[optional]] send_email_to_provider : bool	//! Optional. Pass True if the user's email address should be sent to provider
    [[optional]] is_flexible : bool	//! Optional. Pass True if the final price depends on the shipping method
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard. If empty, one 'Pay total price' button will be shown. If not empty, the first button must be a Pay button.

def from_JV ( var v:JsonValue?; t:sendinvoice ) : sendinvoice
    var q : sendinvoice
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<string>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.payload, _::from_JV((v as _object)["payload"], type<string>))
    move_to_ref(q.provider_token, _::from_JV((v as _object)["provider_token"], type<string>))
    move_to_ref(q.currency, _::from_JV((v as _object)["currency"], type<string>))
    move_to_ref(q.prices, _::from_JV((v as _object)["prices"], type<array<labeledprice>>))
    move_to_ref(q.max_tip_amount, _::from_JV((v as _object)["max_tip_amount"], type<int>))
    move_to_ref(q.suggested_tip_amounts, _::from_JV((v as _object)["suggested_tip_amounts"], type<array<int>>))
    move_to_ref(q.start_parameter, _::from_JV((v as _object)["start_parameter"], type<string>))
    move_to_ref(q.provider_data, _::from_JV((v as _object)["provider_data"], type<string>))
    move_to_ref(q.photo_url, _::from_JV((v as _object)["photo_url"], type<string>))
    move_to_ref(q.photo_size, _::from_JV((v as _object)["photo_size"], type<int>))
    move_to_ref(q.photo_width, _::from_JV((v as _object)["photo_width"], type<int>))
    move_to_ref(q.photo_height, _::from_JV((v as _object)["photo_height"], type<int>))
    move_to_ref(q.need_name, _::from_JV((v as _object)["need_name"], type<bool>))
    move_to_ref(q.need_phone_number, _::from_JV((v as _object)["need_phone_number"], type<bool>))
    move_to_ref(q.need_email, _::from_JV((v as _object)["need_email"], type<bool>))
    move_to_ref(q.need_shipping_address, _::from_JV((v as _object)["need_shipping_address"], type<bool>))
    move_to_ref(q.send_phone_number_to_provider, _::from_JV((v as _object)["send_phone_number_to_provider"], type<bool>))
    move_to_ref(q.send_email_to_provider, _::from_JV((v as _object)["send_email_to_provider"], type<bool>))
    move_to_ref(q.is_flexible, _::from_JV((v as _object)["is_flexible"], type<bool>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendinvoice? ) : sendinvoice?
    return null if v==null
    var q = new sendinvoice
    *q <- from_JV(v,type<sendinvoice>)
    return q

struct createinvoicelink
    //!  Use this method to create a link for an invoice. Returns the created invoice link as String on success.
    title : string	//! Required. Product name, 1-32 characters
    description : string	//! Required. Product description, 1-255 characters
    payload : string	//! Required. Bot-defined invoice payload, 1-128 bytes. This will not be displayed to the user, use for your internal processes.
    provider_token : string	//! Required. Payment provider token, obtained via BotFather
    currency : string	//! Required. Three-letter ISO 4217 currency code, see more on currencies
    prices : array<labeledprice>	//! Required. Price breakdown, a JSON-serialized list of components (e.g. product price, tax, discount, delivery cost, delivery tax, bonus, etc.)
    [[optional]] max_tip_amount : int	//! Optional. The maximum accepted amount for tips in the smallest units of the currency (integer, not float/double). For example, for a maximum tip of US$ 1.45 pass max_tip_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies). Defaults to 0
    [[optional]] suggested_tip_amounts : array<int>	//! Optional. A JSON-serialized array of suggested amounts of tips in the smallest units of the currency (integer, not float/double). At most 4 suggested tip amounts can be specified. The suggested tip amounts must be positive, passed in a strictly increased order and must not exceed max_tip_amount.
    [[optional]] provider_data : string	//! Optional. JSON-serialized data about the invoice, which will be shared with the payment provider. A detailed description of required fields should be provided by the payment provider.
    [[optional]] photo_url : string	//! Optional. URL of the product photo for the invoice. Can be a photo of the goods or a marketing image for a service.
    [[optional]] photo_size : int	//! Optional. Photo size in bytes
    [[optional]] photo_width : int	//! Optional. Photo width
    [[optional]] photo_height : int	//! Optional. Photo height
    [[optional]] need_name : bool	//! Optional. Pass True if you require the user's full name to complete the order
    [[optional]] need_phone_number : bool	//! Optional. Pass True if you require the user's phone number to complete the order
    [[optional]] need_email : bool	//! Optional. Pass True if you require the user's email address to complete the order
    [[optional]] need_shipping_address : bool	//! Optional. Pass True if you require the user's shipping address to complete the order
    [[optional]] send_phone_number_to_provider : bool	//! Optional. Pass True if the user's phone number should be sent to the provider
    [[optional]] send_email_to_provider : bool	//! Optional. Pass True if the user's email address should be sent to the provider
    [[optional]] is_flexible : bool	//! Optional. Pass True if the final price depends on the shipping method

def from_JV ( var v:JsonValue?; t:createinvoicelink ) : createinvoicelink
    var q : createinvoicelink
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.payload, _::from_JV((v as _object)["payload"], type<string>))
    move_to_ref(q.provider_token, _::from_JV((v as _object)["provider_token"], type<string>))
    move_to_ref(q.currency, _::from_JV((v as _object)["currency"], type<string>))
    move_to_ref(q.prices, _::from_JV((v as _object)["prices"], type<array<labeledprice>>))
    move_to_ref(q.max_tip_amount, _::from_JV((v as _object)["max_tip_amount"], type<int>))
    move_to_ref(q.suggested_tip_amounts, _::from_JV((v as _object)["suggested_tip_amounts"], type<array<int>>))
    move_to_ref(q.provider_data, _::from_JV((v as _object)["provider_data"], type<string>))
    move_to_ref(q.photo_url, _::from_JV((v as _object)["photo_url"], type<string>))
    move_to_ref(q.photo_size, _::from_JV((v as _object)["photo_size"], type<int>))
    move_to_ref(q.photo_width, _::from_JV((v as _object)["photo_width"], type<int>))
    move_to_ref(q.photo_height, _::from_JV((v as _object)["photo_height"], type<int>))
    move_to_ref(q.need_name, _::from_JV((v as _object)["need_name"], type<bool>))
    move_to_ref(q.need_phone_number, _::from_JV((v as _object)["need_phone_number"], type<bool>))
    move_to_ref(q.need_email, _::from_JV((v as _object)["need_email"], type<bool>))
    move_to_ref(q.need_shipping_address, _::from_JV((v as _object)["need_shipping_address"], type<bool>))
    move_to_ref(q.send_phone_number_to_provider, _::from_JV((v as _object)["send_phone_number_to_provider"], type<bool>))
    move_to_ref(q.send_email_to_provider, _::from_JV((v as _object)["send_email_to_provider"], type<bool>))
    move_to_ref(q.is_flexible, _::from_JV((v as _object)["is_flexible"], type<bool>))
    return <- q

def from_JV ( var v:JsonValue?; t:createinvoicelink? ) : createinvoicelink?
    return null if v==null
    var q = new createinvoicelink
    *q <- from_JV(v,type<createinvoicelink>)
    return q

struct answershippingquery
    //!  If you sent an invoice requesting a shipping address and the parameter is_flexible was specified, the Bot API will send an Update with a shipping_query field to the bot. Use this method to reply to shipping queries. On success, True is returned.
    shipping_query_id : string	//! Required. Unique identifier for the query to be answered
    ok : bool	//! Required. Pass True if delivery to the specified address is possible and False if there are any problems (for example, if delivery to the specified address is not possible)
    [[optional]] shipping_options : array<shippingoption>	//! Optional. Required if ok is True. A JSON-serialized array of available shipping options.
    [[optional]] error_message : string	//! Optional. Required if ok is False. Error message in human readable form that explains why it is impossible to complete the order (e.g. "Sorry, delivery to your desired address is unavailable'). Telegram will display this message to the user.

def from_JV ( var v:JsonValue?; t:answershippingquery ) : answershippingquery
    var q : answershippingquery
    move_to_ref(q.shipping_query_id, _::from_JV((v as _object)["shipping_query_id"], type<string>))
    move_to_ref(q.ok, _::from_JV((v as _object)["ok"], type<bool>))
    move_to_ref(q.shipping_options, _::from_JV((v as _object)["shipping_options"], type<array<shippingoption>>))
    move_to_ref(q.error_message, _::from_JV((v as _object)["error_message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:answershippingquery? ) : answershippingquery?
    return null if v==null
    var q = new answershippingquery
    *q <- from_JV(v,type<answershippingquery>)
    return q

struct answerprecheckoutquery
    //!  Once the user has confirmed their payment and shipping details, the Bot API sends the final confirmation in the form of an Update with the field pre_checkout_query. Use this method to respond to such pre-checkout queries. On success, True is returned. Note: The Bot API must receive an answer within 10 seconds after the pre-checkout query was sent.
    pre_checkout_query_id : string	//! Required. Unique identifier for the query to be answered
    ok : bool	//! Required. Specify True if everything is alright (goods are available, etc.) and the bot is ready to proceed with the order. Use False if there are any problems.
    [[optional]] error_message : string	//! Optional. Required if ok is False. Error message in human readable form that explains the reason for failure to proceed with the checkout (e.g. "Sorry, somebody just bought the last of our amazing black T-shirts while you were busy filling out your payment details. Please choose a different color or garment!"). Telegram will display this message to the user.

def from_JV ( var v:JsonValue?; t:answerprecheckoutquery ) : answerprecheckoutquery
    var q : answerprecheckoutquery
    move_to_ref(q.pre_checkout_query_id, _::from_JV((v as _object)["pre_checkout_query_id"], type<string>))
    move_to_ref(q.ok, _::from_JV((v as _object)["ok"], type<bool>))
    move_to_ref(q.error_message, _::from_JV((v as _object)["error_message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:answerprecheckoutquery? ) : answerprecheckoutquery?
    return null if v==null
    var q = new answerprecheckoutquery
    *q <- from_JV(v,type<answerprecheckoutquery>)
    return q

struct labeledprice
    //!  This object represents a portion of the price for goods or services.
    _label : string	//! Portion label
    amount : int	//! Price of the product in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).

def from_JV ( var v:JsonValue?; t:labeledprice ) : labeledprice
    var q : labeledprice
    move_to_ref(q._label, _::from_JV((v as _object)["_label"], type<string>))
    move_to_ref(q.amount, _::from_JV((v as _object)["amount"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:labeledprice? ) : labeledprice?
    return null if v==null
    var q = new labeledprice
    *q <- from_JV(v,type<labeledprice>)
    return q

struct invoice
    //!  This object contains basic information about an invoice.
    title : string	//! Product name
    description : string	//! Product description
    start_parameter : string	//! Unique bot deep-linking parameter that can be used to generate this invoice
    currency : string	//! Three-letter ISO 4217 currency code
    total_amount : int	//! Total price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).

def from_JV ( var v:JsonValue?; t:invoice ) : invoice
    var q : invoice
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.start_parameter, _::from_JV((v as _object)["start_parameter"], type<string>))
    move_to_ref(q.currency, _::from_JV((v as _object)["currency"], type<string>))
    move_to_ref(q.total_amount, _::from_JV((v as _object)["total_amount"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:invoice? ) : invoice?
    return null if v==null
    var q = new invoice
    *q <- from_JV(v,type<invoice>)
    return q

struct shippingaddress
    //!  This object represents a shipping address.
    country_code : string	//! Two-letter ISO 3166-1 alpha-2 country code
    state : string	//! State, if applicable
    city : string	//! City
    street_line1 : string	//! First line for the address
    street_line2 : string	//! Second line for the address
    post_code : string	//! Address post code

def from_JV ( var v:JsonValue?; t:shippingaddress ) : shippingaddress
    var q : shippingaddress
    move_to_ref(q.country_code, _::from_JV((v as _object)["country_code"], type<string>))
    move_to_ref(q.state, _::from_JV((v as _object)["state"], type<string>))
    move_to_ref(q.city, _::from_JV((v as _object)["city"], type<string>))
    move_to_ref(q.street_line1, _::from_JV((v as _object)["street_line1"], type<string>))
    move_to_ref(q.street_line2, _::from_JV((v as _object)["street_line2"], type<string>))
    move_to_ref(q.post_code, _::from_JV((v as _object)["post_code"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:shippingaddress? ) : shippingaddress?
    return null if v==null
    var q = new shippingaddress
    *q <- from_JV(v,type<shippingaddress>)
    return q

struct orderinfo
    //!  This object represents information about an order.
    [[optional]] name : string	//! Optional. User name
    [[optional]] phone_number : string	//! Optional. User's phone number
    [[optional]] email : string	//! Optional. User email
    [[optional]] shipping_address : shippingaddress?	//! Optional. User shipping address

def from_JV ( var v:JsonValue?; t:orderinfo ) : orderinfo
    var q : orderinfo
    move_to_ref(q.name, _::from_JV((v as _object)["name"], type<string>))
    move_to_ref(q.phone_number, _::from_JV((v as _object)["phone_number"], type<string>))
    move_to_ref(q.email, _::from_JV((v as _object)["email"], type<string>))
    move_to_ref(q.shipping_address, _::from_JV((v as _object)["shipping_address"], type<shippingaddress?>))
    return <- q

def from_JV ( var v:JsonValue?; t:orderinfo? ) : orderinfo?
    return null if v==null
    var q = new orderinfo
    *q <- from_JV(v,type<orderinfo>)
    return q

struct shippingoption
    //!  This object represents one shipping option.
    id : string	//! Shipping option identifier
    title : string	//! Option title
    prices : array<labeledprice>	//! List of price portions

def from_JV ( var v:JsonValue?; t:shippingoption ) : shippingoption
    var q : shippingoption
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.prices, _::from_JV((v as _object)["prices"], type<array<labeledprice>>))
    return <- q

def from_JV ( var v:JsonValue?; t:shippingoption? ) : shippingoption?
    return null if v==null
    var q = new shippingoption
    *q <- from_JV(v,type<shippingoption>)
    return q

struct successfulpayment
    //!  This object contains basic information about a successful payment.
    currency : string	//! Three-letter ISO 4217 currency code
    total_amount : int	//! Total price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).
    invoice_payload : string	//! Bot specified invoice payload
    [[optional]] shipping_option_id : string	//! Optional. Identifier of the shipping option chosen by the user
    [[optional]] order_info : orderinfo?	//! Optional. Order information provided by the user
    telegram_payment_charge_id : string	//! Telegram payment identifier
    provider_payment_charge_id : string	//! Provider payment identifier

def from_JV ( var v:JsonValue?; t:successfulpayment ) : successfulpayment
    var q : successfulpayment
    move_to_ref(q.currency, _::from_JV((v as _object)["currency"], type<string>))
    move_to_ref(q.total_amount, _::from_JV((v as _object)["total_amount"], type<int>))
    move_to_ref(q.invoice_payload, _::from_JV((v as _object)["invoice_payload"], type<string>))
    move_to_ref(q.shipping_option_id, _::from_JV((v as _object)["shipping_option_id"], type<string>))
    move_to_ref(q.order_info, _::from_JV((v as _object)["order_info"], type<orderinfo?>))
    move_to_ref(q.telegram_payment_charge_id, _::from_JV((v as _object)["telegram_payment_charge_id"], type<string>))
    move_to_ref(q.provider_payment_charge_id, _::from_JV((v as _object)["provider_payment_charge_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:successfulpayment? ) : successfulpayment?
    return null if v==null
    var q = new successfulpayment
    *q <- from_JV(v,type<successfulpayment>)
    return q

struct shippingquery
    //!  This object contains information about an incoming shipping query.
    id : string	//! Unique query identifier
    from : user	//! User who sent the query
    invoice_payload : string	//! Bot specified invoice payload
    shipping_address : shippingaddress	//! User specified shipping address

def from_JV ( var v:JsonValue?; t:shippingquery ) : shippingquery
    var q : shippingquery
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.invoice_payload, _::from_JV((v as _object)["invoice_payload"], type<string>))
    move_to_ref(q.shipping_address, _::from_JV((v as _object)["shipping_address"], type<shippingaddress>))
    return <- q

def from_JV ( var v:JsonValue?; t:shippingquery? ) : shippingquery?
    return null if v==null
    var q = new shippingquery
    *q <- from_JV(v,type<shippingquery>)
    return q

struct precheckoutquery
    //!  This object contains information about an incoming pre-checkout query.
    id : string	//! Unique query identifier
    from : user	//! User who sent the query
    currency : string	//! Three-letter ISO 4217 currency code
    total_amount : int	//! Total price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).
    invoice_payload : string	//! Bot specified invoice payload
    [[optional]] shipping_option_id : string	//! Optional. Identifier of the shipping option chosen by the user
    [[optional]] order_info : orderinfo?	//! Optional. Order information provided by the user

def from_JV ( var v:JsonValue?; t:precheckoutquery ) : precheckoutquery
    var q : precheckoutquery
    move_to_ref(q.id, _::from_JV((v as _object)["id"], type<string>))
    move_to_ref(q.from, _::from_JV((v as _object)["from"], type<user>))
    move_to_ref(q.currency, _::from_JV((v as _object)["currency"], type<string>))
    move_to_ref(q.total_amount, _::from_JV((v as _object)["total_amount"], type<int>))
    move_to_ref(q.invoice_payload, _::from_JV((v as _object)["invoice_payload"], type<string>))
    move_to_ref(q.shipping_option_id, _::from_JV((v as _object)["shipping_option_id"], type<string>))
    move_to_ref(q.order_info, _::from_JV((v as _object)["order_info"], type<orderinfo?>))
    return <- q

def from_JV ( var v:JsonValue?; t:precheckoutquery? ) : precheckoutquery?
    return null if v==null
    var q = new precheckoutquery
    *q <- from_JV(v,type<precheckoutquery>)
    return q

struct passportdata
    //!  Describes Telegram Passport data shared with the bot by the user.
    data : array<encryptedpassportelement>	//! Array with information about documents and other Telegram Passport elements that was shared with the bot
    credentials : encryptedcredentials	//! Encrypted credentials required to decrypt the data

def from_JV ( var v:JsonValue?; t:passportdata ) : passportdata
    var q : passportdata
    move_to_ref(q.data, _::from_JV((v as _object)["data"], type<array<encryptedpassportelement>>))
    move_to_ref(q.credentials, _::from_JV((v as _object)["credentials"], type<encryptedcredentials>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportdata? ) : passportdata?
    return null if v==null
    var q = new passportdata
    *q <- from_JV(v,type<passportdata>)
    return q

struct passportfile
    //!  This object represents a file uploaded to Telegram Passport. Currently all Telegram Passport files are in JPEG format when decrypted and don't exceed 10MB.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    file_size : int	//! File size in bytes
    file_date : int	//! Unix time when the file was uploaded

def from_JV ( var v:JsonValue?; t:passportfile ) : passportfile
    var q : passportfile
    move_to_ref(q.file_id, _::from_JV((v as _object)["file_id"], type<string>))
    move_to_ref(q.file_unique_id, _::from_JV((v as _object)["file_unique_id"], type<string>))
    move_to_ref(q.file_size, _::from_JV((v as _object)["file_size"], type<int>))
    move_to_ref(q.file_date, _::from_JV((v as _object)["file_date"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportfile? ) : passportfile?
    return null if v==null
    var q = new passportfile
    *q <- from_JV(v,type<passportfile>)
    return q

struct encryptedpassportelement
    //!  Describes documents or other Telegram Passport elements shared with the bot by the user.
    _type : string	//! Element type. One of “personal_details”, “passport”, “driver_license”, “identity_card”, “internal_passport”, “address”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”, “phone_number”, “email”.
    [[optional]] data : string	//! Optional. Base64-encoded encrypted Telegram Passport element data provided by the user, available for “personal_details”, “passport”, “driver_license”, “identity_card”, “internal_passport” and “address” types. Can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] phone_number : string	//! Optional. User's verified phone number, available only for “phone_number” type
    [[optional]] email : string	//! Optional. User's verified email address, available only for “email” type
    [[optional]] files : array<passportfile>	//! Optional. Array of encrypted files with documents provided by the user, available for “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration” and “temporary_registration” types. Files can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] front_side : passportfile?	//! Optional. Encrypted file with the front side of the document, provided by the user. Available for “passport”, “driver_license”, “identity_card” and “internal_passport”. The file can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] reverse_side : passportfile?	//! Optional. Encrypted file with the reverse side of the document, provided by the user. Available for “driver_license” and “identity_card”. The file can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] selfie : passportfile?	//! Optional. Encrypted file with the selfie of the user holding a document, provided by the user; available for “passport”, “driver_license”, “identity_card” and “internal_passport”. The file can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] translation : array<passportfile>	//! Optional. Array of encrypted files with translated versions of documents provided by the user. Available if requested for “passport”, “driver_license”, “identity_card”, “internal_passport”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration” and “temporary_registration” types. Files can be decrypted and verified using the accompanying EncryptedCredentials.
    hash : string	//! Base64-encoded element hash for using in PassportElementErrorUnspecified

def from_JV ( var v:JsonValue?; t:encryptedpassportelement ) : encryptedpassportelement
    var q : encryptedpassportelement
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.data, _::from_JV((v as _object)["data"], type<string>))
    move_to_ref(q.phone_number, _::from_JV((v as _object)["phone_number"], type<string>))
    move_to_ref(q.email, _::from_JV((v as _object)["email"], type<string>))
    move_to_ref(q.files, _::from_JV((v as _object)["files"], type<array<passportfile>>))
    move_to_ref(q.front_side, _::from_JV((v as _object)["front_side"], type<passportfile?>))
    move_to_ref(q.reverse_side, _::from_JV((v as _object)["reverse_side"], type<passportfile?>))
    move_to_ref(q.selfie, _::from_JV((v as _object)["selfie"], type<passportfile?>))
    move_to_ref(q.translation, _::from_JV((v as _object)["translation"], type<array<passportfile>>))
    move_to_ref(q.hash, _::from_JV((v as _object)["hash"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:encryptedpassportelement? ) : encryptedpassportelement?
    return null if v==null
    var q = new encryptedpassportelement
    *q <- from_JV(v,type<encryptedpassportelement>)
    return q

struct encryptedcredentials
    //!  Describes data required for decrypting and authenticating EncryptedPassportElement. See the Telegram Passport Documentation for a complete description of the data decryption and authentication processes.
    data : string	//! Base64-encoded encrypted JSON-serialized data with unique user's payload, data hashes and secrets required for EncryptedPassportElement decryption and authentication
    hash : string	//! Base64-encoded data hash for data authentication
    secret : string	//! Base64-encoded secret, encrypted with the bot's public RSA key, required for data decryption

def from_JV ( var v:JsonValue?; t:encryptedcredentials ) : encryptedcredentials
    var q : encryptedcredentials
    move_to_ref(q.data, _::from_JV((v as _object)["data"], type<string>))
    move_to_ref(q.hash, _::from_JV((v as _object)["hash"], type<string>))
    move_to_ref(q.secret, _::from_JV((v as _object)["secret"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:encryptedcredentials? ) : encryptedcredentials?
    return null if v==null
    var q = new encryptedcredentials
    *q <- from_JV(v,type<encryptedcredentials>)
    return q

struct setpassportdataerrors
    //!  Informs a user that some of the Telegram Passport elements they provided contains errors. The user will not be able to re-submit their Passport to you until the errors are fixed (the contents of the field for which you returned the error must change). Returns True on success. Use this if the data submitted by the user doesn't satisfy the standards your service requires for any reason. For example, if a birthday date seems invalid, a submitted document is blurry, a scan shows evidence of tampering, etc. Supply some details in the error message to make sure the user knows how to correct the issues.
    user_id : int	//! Required. User identifier
    errors : array<passportelementerror>	//! Required. A JSON-serialized array describing the errors

def from_JV ( var v:JsonValue?; t:setpassportdataerrors ) : setpassportdataerrors
    var q : setpassportdataerrors
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.errors, _::from_JV((v as _object)["errors"], type<array<passportelementerror>>))
    return <- q

def from_JV ( var v:JsonValue?; t:setpassportdataerrors? ) : setpassportdataerrors?
    return null if v==null
    var q = new setpassportdataerrors
    *q <- from_JV(v,type<setpassportdataerrors>)
    return q

struct passportelementerror
    //!  This object represents an error in the Telegram Passport element which was submitted that should be resolved by the user. It should be one of:
    object_passportelementerror_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:passportelementerror ) : passportelementerror
    var q : passportelementerror
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerror? ) : passportelementerror?
    return null if v==null
    var q = new passportelementerror
    *q <- from_JV(v,type<passportelementerror>)
    return q

struct passportelementerrordatafield
    //!  Represents an issue in one of the data fields that was provided by the user. The error is considered resolved when the field's value changes.
    source : string	//! Error source, must be data
    _type : string	//! The section of the user's Telegram Passport which has the error, one of “personal_details”, “passport”, “driver_license”, “identity_card”, “internal_passport”, “address”
    field_name : string	//! Name of the data field which has the error
    data_hash : string	//! Base64-encoded data hash
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrordatafield ) : passportelementerrordatafield
    var q : passportelementerrordatafield
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.field_name, _::from_JV((v as _object)["field_name"], type<string>))
    move_to_ref(q.data_hash, _::from_JV((v as _object)["data_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrordatafield? ) : passportelementerrordatafield?
    return null if v==null
    var q = new passportelementerrordatafield
    *q <- from_JV(v,type<passportelementerrordatafield>)
    return q

struct passportelementerrorfrontside
    //!  Represents an issue with the front side of a document. The error is considered resolved when the file with the front side of the document changes.
    source : string	//! Error source, must be front_side
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”
    file_hash : string	//! Base64-encoded hash of the file with the front side of the document
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrorfrontside ) : passportelementerrorfrontside
    var q : passportelementerrorfrontside
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hash, _::from_JV((v as _object)["file_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrorfrontside? ) : passportelementerrorfrontside?
    return null if v==null
    var q = new passportelementerrorfrontside
    *q <- from_JV(v,type<passportelementerrorfrontside>)
    return q

struct passportelementerrorreverseside
    //!  Represents an issue with the reverse side of a document. The error is considered resolved when the file with reverse side of the document changes.
    source : string	//! Error source, must be reverse_side
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “driver_license”, “identity_card”
    file_hash : string	//! Base64-encoded hash of the file with the reverse side of the document
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrorreverseside ) : passportelementerrorreverseside
    var q : passportelementerrorreverseside
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hash, _::from_JV((v as _object)["file_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrorreverseside? ) : passportelementerrorreverseside?
    return null if v==null
    var q = new passportelementerrorreverseside
    *q <- from_JV(v,type<passportelementerrorreverseside>)
    return q

struct passportelementerrorselfie
    //!  Represents an issue with the selfie with a document. The error is considered resolved when the file with the selfie changes.
    source : string	//! Error source, must be selfie
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”
    file_hash : string	//! Base64-encoded hash of the file with the selfie
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrorselfie ) : passportelementerrorselfie
    var q : passportelementerrorselfie
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hash, _::from_JV((v as _object)["file_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrorselfie? ) : passportelementerrorselfie?
    return null if v==null
    var q = new passportelementerrorselfie
    *q <- from_JV(v,type<passportelementerrorselfie>)
    return q

struct passportelementerrorfile
    //!  Represents an issue with a document scan. The error is considered resolved when the file with the document scan changes.
    source : string	//! Error source, must be file
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hash : string	//! Base64-encoded file hash
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrorfile ) : passportelementerrorfile
    var q : passportelementerrorfile
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hash, _::from_JV((v as _object)["file_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrorfile? ) : passportelementerrorfile?
    return null if v==null
    var q = new passportelementerrorfile
    *q <- from_JV(v,type<passportelementerrorfile>)
    return q

struct passportelementerrorfiles
    //!  Represents an issue with a list of scans. The error is considered resolved when the list of files containing the scans changes.
    source : string	//! Error source, must be files
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hashes : array<string>	//! List of base64-encoded file hashes
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrorfiles ) : passportelementerrorfiles
    var q : passportelementerrorfiles
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hashes, _::from_JV((v as _object)["file_hashes"], type<array<string>>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrorfiles? ) : passportelementerrorfiles?
    return null if v==null
    var q = new passportelementerrorfiles
    *q <- from_JV(v,type<passportelementerrorfiles>)
    return q

struct passportelementerrortranslationfile
    //!  Represents an issue with one of the files that constitute the translation of a document. The error is considered resolved when the file changes.
    source : string	//! Error source, must be translation_file
    _type : string	//! Type of element of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hash : string	//! Base64-encoded file hash
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrortranslationfile ) : passportelementerrortranslationfile
    var q : passportelementerrortranslationfile
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hash, _::from_JV((v as _object)["file_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrortranslationfile? ) : passportelementerrortranslationfile?
    return null if v==null
    var q = new passportelementerrortranslationfile
    *q <- from_JV(v,type<passportelementerrortranslationfile>)
    return q

struct passportelementerrortranslationfiles
    //!  Represents an issue with the translated version of a document. The error is considered resolved when a file with the document translation change.
    source : string	//! Error source, must be translation_files
    _type : string	//! Type of element of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hashes : array<string>	//! List of base64-encoded file hashes
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrortranslationfiles ) : passportelementerrortranslationfiles
    var q : passportelementerrortranslationfiles
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.file_hashes, _::from_JV((v as _object)["file_hashes"], type<array<string>>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrortranslationfiles? ) : passportelementerrortranslationfiles?
    return null if v==null
    var q = new passportelementerrortranslationfiles
    *q <- from_JV(v,type<passportelementerrortranslationfiles>)
    return q

struct passportelementerrorunspecified
    //!  Represents an issue in an unspecified place. The error is considered resolved when new data is added.
    source : string	//! Error source, must be unspecified
    _type : string	//! Type of element of the user's Telegram Passport which has the issue
    element_hash : string	//! Base64-encoded element hash
    message : string	//! Error message

def from_JV ( var v:JsonValue?; t:passportelementerrorunspecified ) : passportelementerrorunspecified
    var q : passportelementerrorunspecified
    move_to_ref(q.source, _::from_JV((v as _object)["source"], type<string>))
    move_to_ref(q._type, _::from_JV((v as _object)["_type"], type<string>))
    move_to_ref(q.element_hash, _::from_JV((v as _object)["element_hash"], type<string>))
    move_to_ref(q.message, _::from_JV((v as _object)["message"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:passportelementerrorunspecified? ) : passportelementerrorunspecified?
    return null if v==null
    var q = new passportelementerrorunspecified
    *q <- from_JV(v,type<passportelementerrorunspecified>)
    return q

struct sendgame
    //!  Use this method to send a game. On success, the sent Message is returned.
    chat_id : int	//! Required. Unique identifier for the target chat
    [[optional]] message_thread_id : int	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    game_short_name : string	//! Required. Short name of the game, serves as the unique identifier for the game. Set up your games via @BotFather.
    [[optional]] disable_notification : bool	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard. If empty, one 'Play game_title' button will be shown. If not empty, the first button must launch the game.

def from_JV ( var v:JsonValue?; t:sendgame ) : sendgame
    var q : sendgame
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<int>))
    move_to_ref(q.message_thread_id, _::from_JV((v as _object)["message_thread_id"], type<int>))
    move_to_ref(q.game_short_name, _::from_JV((v as _object)["game_short_name"], type<string>))
    move_to_ref(q.disable_notification, _::from_JV((v as _object)["disable_notification"], type<bool>))
    move_to_ref(q.protect_content, _::from_JV((v as _object)["protect_content"], type<bool>))
    move_to_ref(q.reply_to_message_id, _::from_JV((v as _object)["reply_to_message_id"], type<int>))
    move_to_ref(q.allow_sending_without_reply, _::from_JV((v as _object)["allow_sending_without_reply"], type<bool>))
    move_to_ref(q.reply_markup, _::from_JV((v as _object)["reply_markup"], type<inlinekeyboardmarkup?>))
    return <- q

def from_JV ( var v:JsonValue?; t:sendgame? ) : sendgame?
    return null if v==null
    var q = new sendgame
    *q <- from_JV(v,type<sendgame>)
    return q

struct game
    //!  This object represents a game. Use BotFather to create and edit games, their short names will act as unique identifiers.
    title : string	//! Title of the game
    description : string	//! Description of the game
    photo : array<photosize>	//! Photo that will be displayed in the game message in chats.
    [[optional]] text : string	//! Optional. Brief description of the game or high scores included in the game message. Can be automatically edited to include current high scores for the game when the bot calls setGameScore, or manually edited using editMessageText. 0-4096 characters.
    [[optional]] text_entities : array<messageentity>	//! Optional. Special entities that appear in text, such as usernames, URLs, bot commands, etc.
    [[optional]] animation : animation?	//! Optional. Animation that will be displayed in the game message in chats. Upload via BotFather

def from_JV ( var v:JsonValue?; t:game ) : game
    var q : game
    move_to_ref(q.title, _::from_JV((v as _object)["title"], type<string>))
    move_to_ref(q.description, _::from_JV((v as _object)["description"], type<string>))
    move_to_ref(q.photo, _::from_JV((v as _object)["photo"], type<array<photosize>>))
    move_to_ref(q.text, _::from_JV((v as _object)["text"], type<string>))
    move_to_ref(q.text_entities, _::from_JV((v as _object)["text_entities"], type<array<messageentity>>))
    move_to_ref(q.animation, _::from_JV((v as _object)["animation"], type<animation?>))
    return <- q

def from_JV ( var v:JsonValue?; t:game? ) : game?
    return null if v==null
    var q = new game
    *q <- from_JV(v,type<game>)
    return q

struct callbackgame
    //!  A placeholder, currently holds no information. Use BotFather to set up your game.
    object_callbackgame_has_no_fields:bool

def from_JV ( var v:JsonValue?; t:callbackgame ) : callbackgame
    var q : callbackgame
    return <- q

def from_JV ( var v:JsonValue?; t:callbackgame? ) : callbackgame?
    return null if v==null
    var q = new callbackgame
    *q <- from_JV(v,type<callbackgame>)
    return q

struct setgamescore
    //!  Use this method to set the score of the specified user in a game message. On success, if the message is not an inline message, the Message is returned, otherwise True is returned. Returns an error, if the new score is not greater than the user's current score in the chat and force is False.
    user_id : int	//! Required. User identifier
    score : int	//! Required. New score, must be non-negative
    [[optional]] force : bool	//! Optional. Pass True if the high score is allowed to decrease. This can be useful when fixing mistakes or banning cheaters
    [[optional]] disable_edit_message : bool	//! Optional. Pass True if the game message should not be automatically edited to include the current scoreboard
    [[optional]] chat_id : int	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the sent message
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message

def from_JV ( var v:JsonValue?; t:setgamescore ) : setgamescore
    var q : setgamescore
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.score, _::from_JV((v as _object)["score"], type<int>))
    move_to_ref(q.force, _::from_JV((v as _object)["force"], type<bool>))
    move_to_ref(q.disable_edit_message, _::from_JV((v as _object)["disable_edit_message"], type<bool>))
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<int>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:setgamescore? ) : setgamescore?
    return null if v==null
    var q = new setgamescore
    *q <- from_JV(v,type<setgamescore>)
    return q

struct getgamehighscores
    //!  Use this method to get data for high score tables. Will return the score of the specified user and several of their neighbors in a game. Returns an Array of GameHighScore objects.
    user_id : int	//! Required. Target user id
    [[optional]] chat_id : int	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat
    [[optional]] message_id : int	//! Optional. Required if inline_message_id is not specified. Identifier of the sent message
    [[optional]] inline_message_id : string	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message

def from_JV ( var v:JsonValue?; t:getgamehighscores ) : getgamehighscores
    var q : getgamehighscores
    move_to_ref(q.user_id, _::from_JV((v as _object)["user_id"], type<int>))
    move_to_ref(q.chat_id, _::from_JV((v as _object)["chat_id"], type<int>))
    move_to_ref(q.message_id, _::from_JV((v as _object)["message_id"], type<int>))
    move_to_ref(q.inline_message_id, _::from_JV((v as _object)["inline_message_id"], type<string>))
    return <- q

def from_JV ( var v:JsonValue?; t:getgamehighscores? ) : getgamehighscores?
    return null if v==null
    var q = new getgamehighscores
    *q <- from_JV(v,type<getgamehighscores>)
    return q

struct gamehighscore
    //!  This object represents one row of the high scores table for a game.
    position : int	//! Position in high score table for the game
    user : user	//! User
    score : int	//! Score

def from_JV ( var v:JsonValue?; t:gamehighscore ) : gamehighscore
    var q : gamehighscore
    move_to_ref(q.position, _::from_JV((v as _object)["position"], type<int>))
    move_to_ref(q.user, _::from_JV((v as _object)["user"], type<user>))
    move_to_ref(q.score, _::from_JV((v as _object)["score"], type<int>))
    return <- q

def from_JV ( var v:JsonValue?; t:gamehighscore? ) : gamehighscore?
    return null if v==null
    var q = new gamehighscore
    *q <- from_JV(v,type<gamehighscore>)
    return q

