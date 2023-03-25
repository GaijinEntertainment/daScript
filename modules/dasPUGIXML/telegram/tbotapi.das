options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module tbotapi public

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

struct getupdates
    //!  Use this method to receive incoming updates using long polling (wiki). Returns an Array of Update objects.
    [[optional]] offset : int = INT_MIN	//! Optional. Identifier of the first update to be returned. Must be greater by one than the highest among the identifiers of previously received updates. By default, updates starting with the earliest unconfirmed update are returned. An update is considered confirmed as soon as getUpdates is called with an offset higher than its update_id. The negative offset can be specified to retrieve updates starting from -offset update from the end of the updates queue. All previous updates will forgotten.
    [[optional]] limit : int = INT_MIN	//! Optional. Limits the number of updates to be retrieved. Values between 1-100 are accepted. Defaults to 100.
    [[optional]] timeout : int = INT_MIN	//! Optional. Timeout in seconds for long polling. Defaults to 0, i.e. usual short polling. Should be positive, short polling should be used for testing purposes only.
    [[optional]] allowed_updates : array<string>	//! Optional. A JSON-serialized list of the update types you want your bot to receive. For example, specify [“message”, “edited_channel_post”, “callback_query”] to only receive updates of these types. See Update for a complete list of available update types. Specify an empty list to receive all update types except chat_member (default). If not specified, the previous setting will be used. Please note that this parameter doesn't affect updates created before the call to the getUpdates, so unwanted updates may be received for a short period of time.

struct setwebhook
    //!  Use this method to specify a URL and receive incoming updates via an outgoing webhook. Whenever there is an update for the bot, we will send an HTTPS POST request to the specified URL, containing a JSON-serialized Update. In case of an unsuccessful request, we will give up after a reasonable amount of attempts. Returns True on success. If you'd like to make sure that the webhook was set by you, you can specify secret data in the parameter secret_token. If specified, the request will contain a header “X-Telegram-Bot-Api-Secret-Token” with the secret token as content.
    url : string	//! Required. HTTPS URL to send updates to. Use an empty string to remove webhook integration
    [[optional]] certificate : inputfile?	//! Optional. Upload your public key certificate so that the root certificate in use can be checked. See our self-signed guide for details.
    [[optional]] ip_address : string = ""	//! Optional. The fixed IP address which will be used to send webhook requests instead of the IP address resolved through DNS
    [[optional]] max_connections : int = INT_MIN	//! Optional. The maximum allowed number of simultaneous HTTPS connections to the webhook for update delivery, 1-100. Defaults to 40. Use lower values to limit the load on your bot's server, and higher values to increase your bot's throughput.
    [[optional]] allowed_updates : array<string>	//! Optional. A JSON-serialized list of the update types you want your bot to receive. For example, specify [“message”, “edited_channel_post”, “callback_query”] to only receive updates of these types. See Update for a complete list of available update types. Specify an empty list to receive all update types except chat_member (default). If not specified, the previous setting will be used. Please note that this parameter doesn't affect updates created before the call to the setWebhook, so unwanted updates may be received for a short period of time.
    [[optional]] drop_pending_updates : bool = false	//! Optional. Pass True to drop all pending updates
    [[optional]] secret_token : string = ""	//! Optional. A secret token to be sent in a header “X-Telegram-Bot-Api-Secret-Token” in every webhook request, 1-256 characters. Only characters A-Z, a-z, 0-9, _ and - are allowed. The header is useful to ensure that the request comes from a webhook set by you.

struct deletewebhook
    //!  Use this method to remove webhook integration if you decide to switch back to getUpdates. Returns True on success.
    [[optional]] drop_pending_updates : bool = false	//! Optional. Pass True to drop all pending updates

struct getwebhookinfo {} //  Use this method to get current webhook status. Requires no parameters. On success, returns a WebhookInfo object. If the bot is using getUpdates, will return an object with the url field empty.

struct webhookinfo
    //!  Describes the current status of a webhook.
    url : string	//! Webhook URL, may be empty if webhook is not set up
    has_custom_certificate : bool	//! True, if a custom certificate was provided for webhook certificate checks
    pending_update_count : int	//! Number of updates awaiting delivery
    [[optional]] ip_address : string = ""	//! Optional. Currently used webhook IP address
    [[optional]] last_error_date : int = INT_MIN	//! Optional. Unix time for the most recent error that happened when trying to deliver an update via webhook
    [[optional]] last_error_message : string = ""	//! Optional. Error message in human-readable format for the most recent error that happened when trying to deliver an update via webhook
    [[optional]] last_synchronization_error_date : int = INT_MIN	//! Optional. Unix time of the most recent error that happened when trying to synchronize available updates with Telegram datacenters
    [[optional]] max_connections : int = INT_MIN	//! Optional. The maximum allowed number of simultaneous HTTPS connections to the webhook for update delivery
    [[optional]] allowed_updates : array<string>	//! Optional. A list of update types the bot is subscribed to. Defaults to all update types except chat_member

struct user
    //!  This object represents a Telegram user or bot.
    id : int	//! Unique identifier for this user or bot. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier.
    is_bot : bool	//! True, if this user is a bot
    first_name : string	//! User's or bot's first name
    [[optional]] last_name : string = ""	//! Optional. User's or bot's last name
    [[optional]] username : string = ""	//! Optional. User's or bot's username
    [[optional]] language_code : string = ""	//! Optional. IETF language tag of the user's language
    [[optional]] is_premium : bool = false	//! Optional. True, if this user is a Telegram Premium user
    [[optional]] added_to_attachment_menu : bool = false	//! Optional. True, if this user added the bot to the attachment menu
    [[optional]] can_join_groups : bool = false	//! Optional. True, if the bot can be invited to groups. Returned only in getMe.
    [[optional]] can_read_all_group_messages : bool = false	//! Optional. True, if privacy mode is disabled for the bot. Returned only in getMe.
    [[optional]] supports_inline_queries : bool = false	//! Optional. True, if the bot supports inline queries. Returned only in getMe.

struct chat
    //!  This object represents a chat.
    id : int	//! Unique identifier for this chat. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    _type : string	//! Type of chat, can be either “private”, “group”, “supergroup” or “channel”
    [[optional]] title : string = ""	//! Optional. Title, for supergroups, channels and group chats
    [[optional]] username : string = ""	//! Optional. Username, for private chats, supergroups and channels if available
    [[optional]] first_name : string = ""	//! Optional. First name of the other party in a private chat
    [[optional]] last_name : string = ""	//! Optional. Last name of the other party in a private chat
    [[optional]] is_forum : bool = false	//! Optional. True, if the supergroup chat is a forum (has topics enabled)
    [[optional]] photo : chatphoto?	//! Optional. Chat photo. Returned only in getChat.
    [[optional]] active_usernames : array<string>	//! Optional. If non-empty, the list of all active chat usernames; for private chats, supergroups and channels. Returned only in getChat.
    [[optional]] emoji_status_custom_emoji_id : string = ""	//! Optional. Custom emoji identifier of emoji status of the other party in a private chat. Returned only in getChat.
    [[optional]] bio : string = ""	//! Optional. Bio of the other party in a private chat. Returned only in getChat.
    [[optional]] has_private_forwards : bool = false	//! Optional. True, if privacy settings of the other party in the private chat allows to use tg://user?id=<user_id> links only in chats with the user. Returned only in getChat.
    [[optional]] has_restricted_voice_and_video_messages : bool = false	//! Optional. True, if the privacy settings of the other party restrict sending voice and video note messages in the private chat. Returned only in getChat.
    [[optional]] join_to_send_messages : bool = false	//! Optional. True, if users need to join the supergroup before they can send messages. Returned only in getChat.
    [[optional]] join_by_request : bool = false	//! Optional. True, if all users directly joining the supergroup need to be approved by supergroup administrators. Returned only in getChat.
    [[optional]] description : string = ""	//! Optional. Description, for groups, supergroups and channel chats. Returned only in getChat.
    [[optional]] invite_link : string = ""	//! Optional. Primary invite link, for groups, supergroups and channel chats. Returned only in getChat.
    [[optional]] pinned_message : message?	//! Optional. The most recent pinned message (by sending date). Returned only in getChat.
    [[optional]] permissions : chatpermissions?	//! Optional. Default chat member permissions, for groups and supergroups. Returned only in getChat.
    [[optional]] slow_mode_delay : int = INT_MIN	//! Optional. For supergroups, the minimum allowed delay between consecutive messages sent by each unpriviledged user; in seconds. Returned only in getChat.
    [[optional]] message_auto_delete_time : int = INT_MIN	//! Optional. The time after which all messages sent to the chat will be automatically deleted; in seconds. Returned only in getChat.
    [[optional]] has_aggressive_anti_spam_enabled : bool = false	//! Optional. True, if aggressive anti-spam checks are enabled in the supergroup. The field is only available to chat administrators. Returned only in getChat.
    [[optional]] has_hidden_members : bool = false	//! Optional. True, if non-administrators can only get the list of bots and administrators in the chat. Returned only in getChat.
    [[optional]] has_protected_content : bool = false	//! Optional. True, if messages from the chat can't be forwarded to other chats. Returned only in getChat.
    [[optional]] sticker_set_name : string = ""	//! Optional. For supergroups, name of group sticker set. Returned only in getChat.
    [[optional]] can_set_sticker_set : bool = false	//! Optional. True, if the bot can change the group sticker set. Returned only in getChat.
    [[optional]] linked_chat_id : int = INT_MIN	//! Optional. Unique identifier for the linked chat, i.e. the discussion group identifier for a channel and vice versa; for supergroups and channel chats. This identifier may be greater than 32 bits and some programming languages may have difficulty/silent defects in interpreting it. But it is smaller than 52 bits, so a signed 64 bit integer or double-precision float type are safe for storing this identifier. Returned only in getChat.
    [[optional]] location : chatlocation?	//! Optional. For supergroups, the location to which the supergroup is connected. Returned only in getChat.

struct message
    //!  This object represents a message.
    message_id : int	//! Unique message identifier inside this chat
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier of a message thread to which the message belongs; for supergroups only
    [[optional]] from : user?	//! Optional. Sender of the message; empty for messages sent to channels. For backward compatibility, the field contains a fake sender user in non-channel chats, if the message was sent on behalf of a chat.
    [[optional]] sender_chat : chat?	//! Optional. Sender of the message, sent on behalf of a chat. For example, the channel itself for channel posts, the supergroup itself for messages from anonymous group administrators, the linked channel for messages automatically forwarded to the discussion group. For backward compatibility, the field from contains a fake sender user in non-channel chats, if the message was sent on behalf of a chat.
    date : int	//! Date the message was sent in Unix time
    chat : chat	//! Conversation the message belongs to
    [[optional]] forward_from : user?	//! Optional. For forwarded messages, sender of the original message
    [[optional]] forward_from_chat : chat?	//! Optional. For messages forwarded from channels or from anonymous administrators, information about the original sender chat
    [[optional]] forward_from_message_id : int = INT_MIN	//! Optional. For messages forwarded from channels, identifier of the original message in the channel
    [[optional]] forward_signature : string = ""	//! Optional. For forwarded messages that were originally sent in channels or by an anonymous chat administrator, signature of the message sender if present
    [[optional]] forward_sender_name : string = ""	//! Optional. Sender's name for messages forwarded from users who disallow adding a link to their account in forwarded messages
    [[optional]] forward_date : int = INT_MIN	//! Optional. For forwarded messages, date the original message was sent in Unix time
    [[optional]] is_topic_message : bool = false	//! Optional. True, if the message is sent to a forum topic
    [[optional]] is_automatic_forward : bool = false	//! Optional. True, if the message is a channel post that was automatically forwarded to the connected discussion group
    [[optional]] reply_to_message : message?	//! Optional. For replies, the original message. Note that the Message object in this field will not contain further reply_to_message fields even if it itself is a reply.
    [[optional]] via_bot : user?	//! Optional. Bot through which the message was sent
    [[optional]] edit_date : int = INT_MIN	//! Optional. Date the message was last edited in Unix time
    [[optional]] has_protected_content : bool = false	//! Optional. True, if the message can't be forwarded
    [[optional]] media_group_id : string = ""	//! Optional. The unique identifier of a media message group this message belongs to
    [[optional]] author_signature : string = ""	//! Optional. Signature of the post author for messages in channels, or the custom title of an anonymous group administrator
    [[optional]] text : string = ""	//! Optional. For text messages, the actual UTF-8 text of the message
    [[optional]] entities : array<messageentity>	//! Optional. For text messages, special entities like usernames, URLs, bot commands, etc. that appear in the text
    [[optional]] animation : animation?	//! Optional. Message is an animation, information about the animation. For backward compatibility, when this field is set, the document field will also be set
    [[optional]] audio : audio?	//! Optional. Message is an audio file, information about the file
    [[optional]] document : document?	//! Optional. Message is a general file, information about the file
    [[optional]] photo : array<photosize>	//! Optional. Message is a photo, available sizes of the photo
    [[optional]] sticker : sticker?	//! Optional. Message is a sticker, information about the sticker
    [[optional]] video : video?	//! Optional. Message is a video, information about the video
    [[optional]] video_note : videonote?	//! Optional. Message is a video note, information about the video message
    [[optional]] voice : voice?	//! Optional. Message is a voice message, information about the file
    [[optional]] caption : string = ""	//! Optional. Caption for the animation, audio, document, photo, video or voice
    [[optional]] caption_entities : array<messageentity>	//! Optional. For messages with a caption, special entities like usernames, URLs, bot commands, etc. that appear in the caption
    [[optional]] has_media_spoiler : bool = false	//! Optional. True, if the message media is covered by a spoiler animation
    [[optional]] contact : contact?	//! Optional. Message is a shared contact, information about the contact
    [[optional]] dice : dice?	//! Optional. Message is a dice with random value
    [[optional]] game : game?	//! Optional. Message is a game, information about the game. More about games »
    [[optional]] poll : poll?	//! Optional. Message is a native poll, information about the poll
    [[optional]] venue : venue?	//! Optional. Message is a venue, information about the venue. For backward compatibility, when this field is set, the location field will also be set
    [[optional]] location : location?	//! Optional. Message is a shared location, information about the location
    [[optional]] new_chat_members : array<user>	//! Optional. New members that were added to the group or supergroup and information about them (the bot itself may be one of these members)
    [[optional]] left_chat_member : user?	//! Optional. A member was removed from the group, information about them (this member may be the bot itself)
    [[optional]] new_chat_title : string = ""	//! Optional. A chat title was changed to this value
    [[optional]] new_chat_photo : array<photosize>	//! Optional. A chat photo was change to this value
    [[optional]] delete_chat_photo : bool = false	//! Optional. Service message: the chat photo was deleted
    [[optional]] group_chat_created : bool = false	//! Optional. Service message: the group has been created
    [[optional]] supergroup_chat_created : bool = false	//! Optional. Service message: the supergroup has been created. This field can't be received in a message coming through updates, because bot can't be a member of a supergroup when it is created. It can only be found in reply_to_message if someone replies to a very first message in a directly created supergroup.
    [[optional]] channel_chat_created : bool = false	//! Optional. Service message: the channel has been created. This field can't be received in a message coming through updates, because bot can't be a member of a channel when it is created. It can only be found in reply_to_message if someone replies to a very first message in a channel.
    [[optional]] message_auto_delete_timer_changed : messageautodeletetimerchanged?	//! Optional. Service message: auto-delete timer settings changed in the chat
    [[optional]] migrate_to_chat_id : int = INT_MIN	//! Optional. The group has been migrated to a supergroup with the specified identifier. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] migrate_from_chat_id : int = INT_MIN	//! Optional. The supergroup has been migrated from a group with the specified identifier. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] pinned_message : message?	//! Optional. Specified message was pinned. Note that the Message object in this field will not contain further reply_to_message fields even if it is itself a reply.
    [[optional]] invoice : invoice?	//! Optional. Message is an invoice for a payment, information about the invoice. More about payments »
    [[optional]] successful_payment : successfulpayment?	//! Optional. Message is a service message about a successful payment, information about the payment. More about payments »
    [[optional]] user_shared : usershared?	//! Optional. Service message: a user was shared with the bot
    [[optional]] chat_shared : chatshared?	//! Optional. Service message: a chat was shared with the bot
    [[optional]] connected_website : string = ""	//! Optional. The domain name of the website on which the user has logged in. More about Telegram Login »
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

struct messageid
    //!  This object represents a unique message identifier.
    message_id : int	//! Unique message identifier

struct messageentity
    //!  This object represents one special entity in a text message. For example, hashtags, usernames, URLs, etc.
    _type : string	//! Type of the entity. Currently, can be “mention” (@username), “hashtag” (#hashtag), “cashtag” ($USD), “bot_command” (/start@jobs_bot), “url” (https://telegram.org), “email” (do-not-reply@telegram.org), “phone_number” (+1-212-555-0123), “bold” (bold text), “italic” (italic text), “underline” (underlined text), “strikethrough” (strikethrough text), “spoiler” (spoiler message), “code” (monowidth string), “pre” (monowidth block), “text_link” (for clickable text URLs), “text_mention” (for users without usernames), “custom_emoji” (for inline custom emoji stickers)
    offset : int	//! Offset in UTF-16 code units to the start of the entity
    length : int	//! Length of the entity in UTF-16 code units
    [[optional]] url : string = ""	//! Optional. For “text_link” only, URL that will be opened after user taps on the text
    [[optional]] user : user?	//! Optional. For “text_mention” only, the mentioned user
    [[optional]] language : string = ""	//! Optional. For “pre” only, the programming language of the entity text
    [[optional]] custom_emoji_id : string = ""	//! Optional. For “custom_emoji” only, unique identifier of the custom emoji. Use getCustomEmojiStickers to get full information about the sticker

struct photosize
    //!  This object represents one size of a photo or a file / sticker thumbnail.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    width : int	//! Photo width
    height : int	//! Photo height
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes

struct animation
    //!  This object represents an animation file (GIF or H.264/MPEG-4 AVC video without sound).
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    width : int	//! Video width as defined by sender
    height : int	//! Video height as defined by sender
    duration : int	//! Duration of the video in seconds as defined by sender
    [[optional]] thumbnail : photosize?	//! Optional. Animation thumbnail as defined by sender
    [[optional]] file_name : string = ""	//! Optional. Original animation filename as defined by sender
    [[optional]] mime_type : string = ""	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

struct audio
    //!  This object represents an audio file to be treated as music by the Telegram clients.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    duration : int	//! Duration of the audio in seconds as defined by sender
    [[optional]] performer : string = ""	//! Optional. Performer of the audio as defined by sender or by audio tags
    [[optional]] title : string = ""	//! Optional. Title of the audio as defined by sender or by audio tags
    [[optional]] file_name : string = ""	//! Optional. Original filename as defined by sender
    [[optional]] mime_type : string = ""	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.
    [[optional]] thumbnail : photosize?	//! Optional. Thumbnail of the album cover to which the music file belongs

struct document
    //!  This object represents a general file (as opposed to photos, voice messages and audio files).
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    [[optional]] thumbnail : photosize?	//! Optional. Document thumbnail as defined by sender
    [[optional]] file_name : string = ""	//! Optional. Original filename as defined by sender
    [[optional]] mime_type : string = ""	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

struct video
    //!  This object represents a video file.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    width : int	//! Video width as defined by sender
    height : int	//! Video height as defined by sender
    duration : int	//! Duration of the video in seconds as defined by sender
    [[optional]] thumbnail : photosize?	//! Optional. Video thumbnail
    [[optional]] file_name : string = ""	//! Optional. Original filename as defined by sender
    [[optional]] mime_type : string = ""	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

struct videonote
    //!  This object represents a video message (available in Telegram apps as of v.4.0).
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    length : int	//! Video width and height (diameter of the video message) as defined by sender
    duration : int	//! Duration of the video in seconds as defined by sender
    [[optional]] thumbnail : photosize?	//! Optional. Video thumbnail
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes

struct voice
    //!  This object represents a voice note.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    duration : int	//! Duration of the audio in seconds as defined by sender
    [[optional]] mime_type : string = ""	//! Optional. MIME type of the file as defined by sender
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.

struct contact
    //!  This object represents a phone contact.
    phone_number : string	//! Contact's phone number
    first_name : string	//! Contact's first name
    [[optional]] last_name : string = ""	//! Optional. Contact's last name
    [[optional]] user_id : int = INT_MIN	//! Optional. Contact's user identifier in Telegram. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] vcard : string = ""	//! Optional. Additional data about the contact in the form of a vCard

struct dice
    //!  This object represents an animated emoji that displays a random value.
    emoji : string	//! Emoji on which the dice throw animation is based
    value : int	//! Value of the dice, 1-6 for “”, “” and “” base emoji, 1-5 for “” and “” base emoji, 1-64 for “” base emoji

struct polloption
    //!  This object contains information about one answer option in a poll.
    text : string	//! Option text, 1-100 characters
    voter_count : int	//! Number of users that voted for this option

struct pollanswer
    //!  This object represents an answer of a user in a non-anonymous poll.
    poll_id : string	//! Unique poll identifier
    user : user	//! The user, who changed the answer to the poll
    option_ids : array<int>	//! 0-based identifiers of answer options, chosen by the user. May be empty if the user retracted their vote.

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
    [[optional]] correct_option_id : int = INT_MIN	//! Optional. 0-based identifier of the correct answer option. Available only for polls in the quiz mode, which are closed, or was sent (not forwarded) by the bot or to the private chat with the bot.
    [[optional]] explanation : string = ""	//! Optional. Text that is shown when a user chooses an incorrect answer or taps on the lamp icon in a quiz-style poll, 0-200 characters
    [[optional]] explanation_entities : array<messageentity>	//! Optional. Special entities like usernames, URLs, bot commands, etc. that appear in the explanation
    [[optional]] open_period : int = INT_MIN	//! Optional. Amount of time in seconds the poll will be active after creation
    [[optional]] close_date : int = INT_MIN	//! Optional. Point in time (Unix timestamp) when the poll will be automatically closed

struct location
    //!  This object represents a point on the map.
    longitude : float	//! Longitude as defined by sender
    latitude : float	//! Latitude as defined by sender
    [[optional]] horizontal_accuracy : float = -FLT_MAX	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int = INT_MIN	//! Optional. Time relative to the message sending date, during which the location can be updated; in seconds. For active live locations only.
    [[optional]] heading : int = INT_MIN	//! Optional. The direction in which user is moving, in degrees; 1-360. For active live locations only.
    [[optional]] proximity_alert_radius : int = INT_MIN	//! Optional. The maximum distance for proximity alerts about approaching another chat member, in meters. For sent live locations only.

struct venue
    //!  This object represents a venue.
    location : location	//! Venue location. Can't be a live location
    title : string	//! Name of the venue
    address : string	//! Address of the venue
    [[optional]] foursquare_id : string = ""	//! Optional. Foursquare identifier of the venue
    [[optional]] foursquare_type : string = ""	//! Optional. Foursquare type of the venue. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string = ""	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string = ""	//! Optional. Google Places type of the venue. (See supported types.)

struct webappdata
    //!  Describes data sent from a Web App to the bot.
    data : string	//! The data. Be aware that a bad client can send arbitrary data in this field.
    button_text : string	//! Text of the web_app keyboard button from which the Web App was opened. Be aware that a bad client can send arbitrary data in this field.

struct proximityalerttriggered
    //!  This object represents the content of a service message, sent whenever a user in the chat triggers a proximity alert set by another user.
    traveler : user	//! User that triggered the alert
    watcher : user	//! User that set the alert
    distance : int	//! The distance between the users

struct messageautodeletetimerchanged
    //!  This object represents a service message about a change in auto-delete timer settings.
    message_auto_delete_time : int	//! New auto-delete time for messages in the chat; in seconds

struct forumtopiccreated
    //!  This object represents a service message about a new forum topic created in the chat.
    name : string	//! Name of the topic
    icon_color : int	//! Color of the topic icon in RGB format
    [[optional]] icon_custom_emoji_id : string = ""	//! Optional. Unique identifier of the custom emoji shown as the topic icon

struct forumtopicclosed {} //  This object represents a service message about a forum topic closed in the chat. Currently holds no information.

struct forumtopicedited
    //!  This object represents a service message about an edited forum topic.
    [[optional]] name : string = ""	//! Optional. New name of the topic, if it was edited
    [[optional]] icon_custom_emoji_id : string = ""	//! Optional. New identifier of the custom emoji shown as the topic icon, if it was edited; an empty string if the icon was removed

struct forumtopicreopened {} //  This object represents a service message about a forum topic reopened in the chat. Currently holds no information.

struct generalforumtopichidden {} //  This object represents a service message about General forum topic hidden in the chat. Currently holds no information.

struct generalforumtopicunhidden {} //  This object represents a service message about General forum topic unhidden in the chat. Currently holds no information.

struct usershared
    //!  This object contains information about the user whose identifier was shared with the bot using a KeyboardButtonRequestUser button.
    request_id : int	//! Identifier of the request
    user_id : int	//! Identifier of the shared user. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier. The bot may not have access to the user and could be unable to use this identifier, unless the user is already known to the bot by some other means.

struct chatshared
    //!  This object contains information about the chat whose identifier was shared with the bot using a KeyboardButtonRequestChat button.
    request_id : int	//! Identifier of the request
    chat_id : int	//! Identifier of the shared chat. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier. The bot may not have access to the chat and could be unable to use this identifier, unless the chat is already known to the bot by some other means.

struct writeaccessallowed {} //  This object represents a service message about a user allowing a bot added to the attachment menu to write messages. Currently holds no information.

struct videochatscheduled
    //!  This object represents a service message about a video chat scheduled in the chat.
    start_date : int	//! Point in time (Unix timestamp) when the video chat is supposed to be started by a chat administrator

struct videochatstarted {} //  This object represents a service message about a video chat started in the chat. Currently holds no information.

struct videochatended
    //!  This object represents a service message about a video chat ended in the chat.
    duration : int	//! Video chat duration in seconds

struct videochatparticipantsinvited
    //!  This object represents a service message about new members invited to a video chat.
    users : array<user>	//! New members that were invited to the video chat

struct userprofilephotos
    //!  This object represent a user's profile pictures.
    total_count : int	//! Total number of profile pictures the target user has
    photos : array<array<photosize>>	//! Requested profile pictures (in up to 4 sizes each)

struct file
    //!  This object represents a file ready to be downloaded. The file can be downloaded via the link https://api.telegram.org/file/bot<token>/<file_path>. It is guaranteed that the link will be valid for at least 1 hour. When the link expires, a new one can be requested by calling getFile.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes. It can be bigger than 2^31 and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this value.
    [[optional]] file_path : string = ""	//! Optional. File path. Use https://api.telegram.org/file/bot<token>/<file_path> to get the file.

struct webappinfo
    //!  Describes a Web App.
    url : string	//! An HTTPS URL of a Web App to be opened with additional data as specified in Initializing Web Apps

struct replykeyboardmarkup
    //!  This object represents a custom keyboard with reply options (see Introduction to bots for details and examples).
    keyboard : array<array<keyboardbutton>>	//! Array of button rows, each represented by an Array of KeyboardButton objects
    [[optional]] is_persistent : bool = false	//! Optional. Requests clients to always show the keyboard when the regular keyboard is hidden. Defaults to false, in which case the custom keyboard can be hidden and opened with a keyboard icon.
    [[optional]] resize_keyboard : bool = false	//! Optional. Requests clients to resize the keyboard vertically for optimal fit (e.g., make the keyboard smaller if there are just two rows of buttons). Defaults to false, in which case the custom keyboard is always of the same height as the app's standard keyboard.
    [[optional]] one_time_keyboard : bool = false	//! Optional. Requests clients to hide the keyboard as soon as it's been used. The keyboard will still be available, but clients will automatically display the usual letter-keyboard in the chat - the user can press a special button in the input field to see the custom keyboard again. Defaults to false.
    [[optional]] input_field_placeholder : string = ""	//! Optional. The placeholder to be shown in the input field when the keyboard is active; 1-64 characters
    [[optional]] selective : bool = false	//! Optional. Use this parameter if you want to show the keyboard to specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.Example: A user requests to change the bot's language, bot replies to the request with a keyboard to select the new language. Other users in the group don't see the keyboard.

struct keyboardbutton
    //!  This object represents one button of the reply keyboard. For simple text buttons, String can be used instead of this object to specify the button text. The optional fields web_app, request_user, request_chat, request_contact, request_location, and request_poll are mutually exclusive.
    text : string	//! Text of the button. If none of the optional fields are used, it will be sent as a message when the button is pressed
    [[optional]] request_user : keyboardbuttonrequestuser?	//! Optional. If specified, pressing the button will open a list of suitable users. Tapping on any user will send their identifier to the bot in a “user_shared” service message. Available in private chats only.
    [[optional]] request_chat : keyboardbuttonrequestchat?	//! Optional. If specified, pressing the button will open a list of suitable chats. Tapping on a chat will send its identifier to the bot in a “chat_shared” service message. Available in private chats only.
    [[optional]] request_contact : bool = false	//! Optional. If True, the user's phone number will be sent as a contact when the button is pressed. Available in private chats only.
    [[optional]] request_location : bool = false	//! Optional. If True, the user's current location will be sent when the button is pressed. Available in private chats only.
    [[optional]] request_poll : keyboardbuttonpolltype?	//! Optional. If specified, the user will be asked to create a poll and send it to the bot when the button is pressed. Available in private chats only.
    [[optional]] web_app : webappinfo?	//! Optional. If specified, the described Web App will be launched when the button is pressed. The Web App will be able to send a “web_app_data” service message. Available in private chats only.

struct keyboardbuttonrequestuser
    //!  This object defines the criteria used to request a suitable user. The identifier of the selected user will be shared with the bot when the corresponding button is pressed. More about requesting users »
    request_id : int	//! Signed 32-bit identifier of the request, which will be received back in the UserShared object. Must be unique within the message
    [[optional]] user_is_bot : bool = false	//! Optional. Pass True to request a bot, pass False to request a regular user. If not specified, no additional restrictions are applied.
    [[optional]] user_is_premium : bool = false	//! Optional. Pass True to request a premium user, pass False to request a non-premium user. If not specified, no additional restrictions are applied.

struct keyboardbuttonrequestchat
    //!  This object defines the criteria used to request a suitable chat. The identifier of the selected chat will be shared with the bot when the corresponding button is pressed. More about requesting chats »
    request_id : int	//! Signed 32-bit identifier of the request, which will be received back in the ChatShared object. Must be unique within the message
    chat_is_channel : bool	//! Pass True to request a channel chat, pass False to request a group or a supergroup chat.
    [[optional]] chat_is_forum : bool = false	//! Optional. Pass True to request a forum supergroup, pass False to request a non-forum chat. If not specified, no additional restrictions are applied.
    [[optional]] chat_has_username : bool = false	//! Optional. Pass True to request a supergroup or a channel with a username, pass False to request a chat without a username. If not specified, no additional restrictions are applied.
    [[optional]] chat_is_created : bool = false	//! Optional. Pass True to request a chat owned by the user. Otherwise, no additional restrictions are applied.
    [[optional]] user_administrator_rights : chatadministratorrights?	//! Optional. A JSON-serialized object listing the required administrator rights of the user in the chat. The rights must be a superset of bot_administrator_rights. If not specified, no additional restrictions are applied.
    [[optional]] bot_administrator_rights : chatadministratorrights?	//! Optional. A JSON-serialized object listing the required administrator rights of the bot in the chat. The rights must be a subset of user_administrator_rights. If not specified, no additional restrictions are applied.
    [[optional]] bot_is_member : bool = false	//! Optional. Pass True to request a chat with the bot as a member. Otherwise, no additional restrictions are applied.

struct keyboardbuttonpolltype
    //!  This object represents type of a poll, which is allowed to be created and sent when the corresponding button is pressed.
    [[optional]] _type : string = ""	//! Optional. If quiz is passed, the user will be allowed to create only polls in the quiz mode. If regular is passed, only regular polls will be allowed. Otherwise, the user will be allowed to create a poll of any type.

struct replykeyboardremove
    //!  Upon receiving a message with this object, Telegram clients will remove the current custom keyboard and display the default letter-keyboard. By default, custom keyboards are displayed until a new keyboard is sent by a bot. An exception is made for one-time keyboards that are hidden immediately after the user presses a button (see ReplyKeyboardMarkup).
    remove_keyboard : bool	//! Requests clients to remove the custom keyboard (user will not be able to summon this keyboard; if you want to hide the keyboard from sight but keep it accessible, use one_time_keyboard in ReplyKeyboardMarkup)
    [[optional]] selective : bool = false	//! Optional. Use this parameter if you want to remove the keyboard for specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.Example: A user votes in a poll, bot returns confirmation message in reply to the vote and removes the keyboard for that user, while still showing the keyboard with poll options to users who haven't voted yet.

struct inlinekeyboardmarkup
    //!  This object represents an inline keyboard that appears right next to the message it belongs to.
    inline_keyboard : array<array<inlinekeyboardbutton>>	//! Array of button rows, each represented by an Array of InlineKeyboardButton objects

struct inlinekeyboardbutton
    //!  This object represents one button of an inline keyboard. You must use exactly one of the optional fields.
    text : string	//! Label text on the button
    [[optional]] url : string = ""	//! Optional. HTTP or tg:// URL to be opened when the button is pressed. Links tg://user?id=<user_id> can be used to mention a user by their ID without using a username, if this is allowed by their privacy settings.
    [[optional]] callback_data : string = ""	//! Optional. Data to be sent in a callback query to the bot when button is pressed, 1-64 bytes
    [[optional]] web_app : webappinfo?	//! Optional. Description of the Web App that will be launched when the user presses the button. The Web App will be able to send an arbitrary message on behalf of the user using the method answerWebAppQuery. Available only in private chats between a user and the bot.
    [[optional]] login_url : loginurl?	//! Optional. An HTTPS URL used to automatically authorize the user. Can be used as a replacement for the Telegram Login Widget.
    [[optional]] switch_inline_query : string = ""	//! Optional. If set, pressing the button will prompt the user to select one of their chats, open that chat and insert the bot's username and the specified inline query in the input field. May be empty, in which case just the bot's username will be inserted.Note: This offers an easy way for users to start using your bot in inline mode when they are currently in a private chat with it. Especially useful when combined with switch_pm… actions - in this case the user will be automatically returned to the chat they switched from, skipping the chat selection screen.
    [[optional]] switch_inline_query_current_chat : string = ""	//! Optional. If set, pressing the button will insert the bot's username and the specified inline query in the current chat's input field. May be empty, in which case only the bot's username will be inserted. This offers a quick way for the user to open your bot in inline mode in the same chat - good for selecting something from multiple options.
    [[optional]] callback_game : callbackgame?	//! Optional. Description of the game that will be launched when the user presses the button.NOTE: This type of button must always be the first button in the first row.
    [[optional]] pay : bool = false	//! Optional. Specify True, to send a Pay button.NOTE: This type of button must always be the first button in the first row and can only be used in invoice messages.

struct loginurl
    //!  This object represents a parameter of the inline keyboard button used to automatically authorize a user. Serves as a great replacement for the Telegram Login Widget when the user is coming from Telegram. All the user needs to do is tap/click a button and confirm that they want to log in: Telegram apps support these buttons as of version 5.7.
    url : string	//! An HTTPS URL to be opened with user authorization data added to the query string when the button is pressed. If the user refuses to provide authorization data, the original URL without information about the user will be opened. The data added is the same as described in Receiving authorization data.NOTE: You must always check the hash of the received data to verify the authentication and the integrity of the data as described in Checking authorization.
    [[optional]] forward_text : string = ""	//! Optional. New text of the button in forwarded messages.
    [[optional]] bot_username : string = ""	//! Optional. Username of a bot, which will be used for user authorization. See Setting up a bot for more details. If not specified, the current bot's username will be assumed. The url's domain must be the same as the domain linked with the bot. See Linking your domain to the bot for more details.
    [[optional]] request_write_access : bool = false	//! Optional. Pass True to request the permission for your bot to send messages to the user.

struct callbackquery
    //!  This object represents an incoming callback query from a callback button in an inline keyboard. If the button that originated the query was attached to a message sent by the bot, the field message will be present. If the button was attached to a message sent via the bot (in inline mode), the field inline_message_id will be present. Exactly one of the fields data or game_short_name will be present.
    id : string	//! Unique identifier for this query
    from : user	//! Sender
    [[optional]] message : message?	//! Optional. Message with the callback button that originated the query. Note that message content and message date will not be available if the message is too old
    [[optional]] inline_message_id : string = ""	//! Optional. Identifier of the message sent via the bot in inline mode, that originated the query.
    chat_instance : string	//! Global identifier, uniquely corresponding to the chat to which the message with the callback button was sent. Useful for high scores in games.
    [[optional]] data : string = ""	//! Optional. Data associated with the callback button. Be aware that the message originated the query can contain no callback buttons with this data.
    [[optional]] game_short_name : string = ""	//! Optional. Short name of a Game to be returned, serves as the unique identifier for the game

struct forcereply
    //!  Upon receiving a message with this object, Telegram clients will display a reply interface to the user (act as if the user has selected the bot's message and tapped 'Reply'). This can be extremely useful if you want to create user-friendly step-by-step interfaces without having to sacrifice privacy mode.
    force_reply : bool	//! Shows reply interface to the user, as if they manually selected the bot's message and tapped 'Reply'
    [[optional]] input_field_placeholder : string = ""	//! Optional. The placeholder to be shown in the input field when the reply is active; 1-64 characters
    [[optional]] selective : bool = false	//! Optional. Use this parameter if you want to force reply from specific users only. Targets: 1) users that are @mentioned in the text of the Message object; 2) if the bot's message is a reply (has reply_to_message_id), sender of the original message.

struct chatphoto
    //!  This object represents a chat photo.
    small_file_id : string	//! File identifier of small (160x160) chat photo. This file_id can be used only for photo download and only for as long as the photo is not changed.
    small_file_unique_id : string	//! Unique file identifier of small (160x160) chat photo, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    big_file_id : string	//! File identifier of big (640x640) chat photo. This file_id can be used only for photo download and only for as long as the photo is not changed.
    big_file_unique_id : string	//! Unique file identifier of big (640x640) chat photo, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.

struct chatinvitelink
    //!  Represents an invite link for a chat.
    invite_link : string	//! The invite link. If the link was created by another chat administrator, then the second part of the link will be replaced with “…”.
    creator : user	//! Creator of the link
    creates_join_request : bool	//! True, if users joining the chat via the link need to be approved by chat administrators
    is_primary : bool	//! True, if the link is primary
    is_revoked : bool	//! True, if the link is revoked
    [[optional]] name : string = ""	//! Optional. Invite link name
    [[optional]] expire_date : int = INT_MIN	//! Optional. Point in time (Unix timestamp) when the link will expire or has been expired
    [[optional]] member_limit : int = INT_MIN	//! Optional. The maximum number of users that can be members of the chat simultaneously after joining the chat via this invite link; 1-99999
    [[optional]] pending_join_request_count : int = INT_MIN	//! Optional. Number of pending join requests created using this link

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
    [[optional]] can_post_messages : bool = false	//! Optional. True, if the administrator can post in the channel; channels only
    [[optional]] can_edit_messages : bool = false	//! Optional. True, if the administrator can edit messages of other users and can pin messages; channels only
    [[optional]] can_pin_messages : bool = false	//! Optional. True, if the user is allowed to pin messages; groups and supergroups only
    [[optional]] can_manage_topics : bool = false	//! Optional. True, if the user is allowed to create, rename, close, and reopen forum topics; supergroups only

struct chatmember {} //  This object contains information about one member of a chat. Currently, the following 6 types of chat members are supported:

struct chatmemberowner
    //!  Represents a chat member that owns the chat and has all administrator privileges.
    status : string	//! The member's status in the chat, always “creator”
    user : user	//! Information about the user
    is_anonymous : bool	//! True, if the user's presence in the chat is hidden
    [[optional]] custom_title : string = ""	//! Optional. Custom title for this user

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
    [[optional]] can_post_messages : bool = false	//! Optional. True, if the administrator can post in the channel; channels only
    [[optional]] can_edit_messages : bool = false	//! Optional. True, if the administrator can edit messages of other users and can pin messages; channels only
    [[optional]] can_pin_messages : bool = false	//! Optional. True, if the user is allowed to pin messages; groups and supergroups only
    [[optional]] can_manage_topics : bool = false	//! Optional. True, if the user is allowed to create, rename, close, and reopen forum topics; supergroups only
    [[optional]] custom_title : string = ""	//! Optional. Custom title for this user

struct chatmembermember
    //!  Represents a chat member that has no additional privileges or restrictions.
    status : string	//! The member's status in the chat, always “member”
    user : user	//! Information about the user

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

struct chatmemberleft
    //!  Represents a chat member that isn't currently a member of the chat, but may join it themselves.
    status : string	//! The member's status in the chat, always “left”
    user : user	//! Information about the user

struct chatmemberbanned
    //!  Represents a chat member that was banned in the chat and can't return to the chat or view chat messages.
    status : string	//! The member's status in the chat, always “kicked”
    user : user	//! Information about the user
    until_date : int	//! Date when restrictions will be lifted for this user; unix time. If 0, then the user is banned forever

struct chatmemberupdated
    //!  This object represents changes in the status of a chat member.
    chat : chat	//! Chat the user belongs to
    from : user	//! Performer of the action, which resulted in the change
    date : int	//! Date the change was done in Unix time
    old_chat_member : chatmember	//! Previous information about the chat member
    new_chat_member : chatmember	//! New information about the chat member
    [[optional]] invite_link : chatinvitelink?	//! Optional. Chat invite link, which was used by the user to join the chat; for joining by invite link events only.

struct chatjoinrequest
    //!  Represents a join request sent to a chat.
    chat : chat	//! Chat to which the request was sent
    from : user	//! User that sent the join request
    user_chat_id : int	//! Identifier of a private chat with the user who sent the join request. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a 64-bit integer or double-precision float type are safe for storing this identifier. The bot can use this identifier for 24 hours to send messages until the join request is processed, assuming no other administrator contacted the user.
    date : int	//! Date the request was sent in Unix time
    [[optional]] bio : string = ""	//! Optional. Bio of the user.
    [[optional]] invite_link : chatinvitelink?	//! Optional. Chat invite link that was used by the user to send the join request

struct chatpermissions
    //!  Describes actions that a non-administrator user is allowed to take in a chat.
    [[optional]] can_send_messages : bool = false	//! Optional. True, if the user is allowed to send text messages, contacts, invoices, locations and venues
    [[optional]] can_send_audios : bool = false	//! Optional. True, if the user is allowed to send audios
    [[optional]] can_send_documents : bool = false	//! Optional. True, if the user is allowed to send documents
    [[optional]] can_send_photos : bool = false	//! Optional. True, if the user is allowed to send photos
    [[optional]] can_send_videos : bool = false	//! Optional. True, if the user is allowed to send videos
    [[optional]] can_send_video_notes : bool = false	//! Optional. True, if the user is allowed to send video notes
    [[optional]] can_send_voice_notes : bool = false	//! Optional. True, if the user is allowed to send voice notes
    [[optional]] can_send_polls : bool = false	//! Optional. True, if the user is allowed to send polls
    [[optional]] can_send_other_messages : bool = false	//! Optional. True, if the user is allowed to send animations, games, stickers and use inline bots
    [[optional]] can_add_web_page_previews : bool = false	//! Optional. True, if the user is allowed to add web page previews to their messages
    [[optional]] can_change_info : bool = false	//! Optional. True, if the user is allowed to change the chat title, photo and other settings. Ignored in public supergroups
    [[optional]] can_invite_users : bool = false	//! Optional. True, if the user is allowed to invite new users to the chat
    [[optional]] can_pin_messages : bool = false	//! Optional. True, if the user is allowed to pin messages. Ignored in public supergroups
    [[optional]] can_manage_topics : bool = false	//! Optional. True, if the user is allowed to create forum topics. If omitted defaults to the value of can_pin_messages

struct chatlocation
    //!  Represents a location to which a chat is connected.
    location : location	//! The location to which the supergroup is connected. Can't be a live location.
    address : string	//! Location address; 1-64 characters, as defined by the chat owner

struct forumtopic
    //!  This object represents a forum topic.
    message_thread_id : int	//! Unique identifier of the forum topic
    name : string	//! Name of the topic
    icon_color : int	//! Color of the topic icon in RGB format
    [[optional]] icon_custom_emoji_id : string = ""	//! Optional. Unique identifier of the custom emoji shown as the topic icon

struct botcommand
    //!  This object represents a bot command.
    command : string	//! Text of the command; 1-32 characters. Can contain only lowercase English letters, digits and underscores.
    description : string	//! Description of the command; 1-256 characters.

struct botcommandscope {} //  This object represents the scope to which bot commands are applied. Currently, the following 7 scopes are supported:

struct determining_list_of_commands {} //  The following algorithm is used to determine the list of commands for a particular user viewing the bot menu. The first list of commands which is set is returned: Commands in the chat with the bot Commands in group and supergroup chats

struct botcommandscopedefault
    //!  Represents the default scope of bot commands. Default commands are used if no commands with a narrower scope are specified for the user.
    _type : string	//! Scope type, must be default

struct botcommandscopeallprivatechats
    //!  Represents the scope of bot commands, covering all private chats.
    _type : string	//! Scope type, must be all_private_chats

struct botcommandscopeallgroupchats
    //!  Represents the scope of bot commands, covering all group and supergroup chats.
    _type : string	//! Scope type, must be all_group_chats

struct botcommandscopeallchatadministrators
    //!  Represents the scope of bot commands, covering all group and supergroup chat administrators.
    _type : string	//! Scope type, must be all_chat_administrators

struct botcommandscopechat
    //!  Represents the scope of bot commands, covering a specific chat.
    _type : string	//! Scope type, must be chat
    chat_id : string	//! Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct botcommandscopechatadministrators
    //!  Represents the scope of bot commands, covering all administrators of a specific group or supergroup chat.
    _type : string	//! Scope type, must be chat_administrators
    chat_id : string	//! Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct botcommandscopechatmember
    //!  Represents the scope of bot commands, covering a specific member of a group or supergroup chat.
    _type : string	//! Scope type, must be chat_member
    chat_id : string	//! Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    user_id : int	//! Unique identifier of the target user

struct botdescription
    //!  This object represents the bot's description.
    description : string	//! The bot's description

struct botshortdescription
    //!  This object represents the bot's short description.
    short_description : string	//! The bot's short description

struct menubutton {} //  This object describes the bot's menu button in a private chat. It should be one of If a menu button other than MenuButtonDefault is set for a private chat, then it is applied in the chat. Otherwise the default menu button is applied. By default, the menu button opens the list of bot commands.

struct menubuttoncommands
    //!  Represents a menu button, which opens the bot's list of commands.
    _type : string	//! Type of the button, must be commands

struct menubuttonwebapp
    //!  Represents a menu button, which launches a Web App.
    _type : string	//! Type of the button, must be web_app
    text : string	//! Text on the button
    web_app : webappinfo	//! Description of the Web App that will be launched when the user presses the button. The Web App will be able to send an arbitrary message on behalf of the user using the method answerWebAppQuery.

struct menubuttondefault
    //!  Describes that no specific value for the menu button was set.
    _type : string	//! Type of the button, must be default

struct responseparameters
    //!  Describes why a request was unsuccessful.
    [[optional]] migrate_to_chat_id : int = INT_MIN	//! Optional. The group has been migrated to a supergroup with the specified identifier. This number may have more than 32 significant bits and some programming languages may have difficulty/silent defects in interpreting it. But it has at most 52 significant bits, so a signed 64-bit integer or double-precision float type are safe for storing this identifier.
    [[optional]] retry_after : int = INT_MIN	//! Optional. In case of exceeding flood control, the number of seconds left to wait before the request can be repeated

struct inputmedia {} //  This object represents the content of a media message to be sent. It should be one of

struct inputmediaphoto
    //!  Represents a photo to be sent.
    _type : string	//! Type of the result, must be photo
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Caption of the photo to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool = false	//! Optional. Pass True if the photo needs to be covered with a spoiler animation

struct inputmediavideo
    //!  Represents a video to be sent.
    _type : string	//! Type of the result, must be video
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Caption of the video to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] width : int = INT_MIN	//! Optional. Video width
    [[optional]] height : int = INT_MIN	//! Optional. Video height
    [[optional]] duration : int = INT_MIN	//! Optional. Video duration in seconds
    [[optional]] supports_streaming : bool = false	//! Optional. Pass True if the uploaded video is suitable for streaming
    [[optional]] has_spoiler : bool = false	//! Optional. Pass True if the video needs to be covered with a spoiler animation

struct inputmediaanimation
    //!  Represents an animation file (GIF or H.264/MPEG-4 AVC video without sound) to be sent.
    _type : string	//! Type of the result, must be animation
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Caption of the animation to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the animation caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] width : int = INT_MIN	//! Optional. Animation width
    [[optional]] height : int = INT_MIN	//! Optional. Animation height
    [[optional]] duration : int = INT_MIN	//! Optional. Animation duration in seconds
    [[optional]] has_spoiler : bool = false	//! Optional. Pass True if the animation needs to be covered with a spoiler animation

struct inputmediaaudio
    //!  Represents an audio file to be treated as music to be sent.
    _type : string	//! Type of the result, must be audio
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Caption of the audio to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] duration : int = INT_MIN	//! Optional. Duration of the audio in seconds
    [[optional]] performer : string = ""	//! Optional. Performer of the audio
    [[optional]] title : string = ""	//! Optional. Title of the audio

struct inputmediadocument
    //!  Represents a general file to be sent.
    _type : string	//! Type of the result, must be document
    media : string	//! File to send. Pass a file_id to send a file that exists on the Telegram servers (recommended), pass an HTTP URL for Telegram to get a file from the Internet, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Caption of the document to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] disable_content_type_detection : bool = false	//! Optional. Disables automatic server-side content type detection for files uploaded using multipart/form-data. Always True, if the document is sent as part of an album.

struct inputfile {} //  This object represents the contents of a file to be uploaded. Must be posted using multipart/form-data in the usual way that files are uploaded via the browser.

struct sending_files {} //  There are three ways to send files (photos, stickers, audio, media, etc.): Sending by file_id Sending by URL

struct inline_mode_objects {} //  Objects and methods used in the inline mode are described in the Inline mode section.

struct getme {} //  A simple method for testing your bot's authentication token. Requires no parameters. Returns basic information about the bot in form of a User object.

struct logout {} //  Use this method to log out from the cloud Bot API server before launching the bot locally. You must log out the bot before running it locally, otherwise there is no guarantee that the bot will receive updates. After a successful call, you can immediately log in on a local server, but will not be able to log in back to the cloud Bot API server for 10 minutes. Returns True on success. Requires no parameters.

struct close {} //  Use this method to close the bot instance before moving it from one local server to another. You need to delete the webhook before calling this method to ensure that the bot isn't launched again after server restart. The method will return error 429 in the first 10 minutes after the bot is launched. Returns True on success. Requires no parameters.

struct sendmessage
    //!  Use this method to send text messages. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    text : string	//! Required. Text of the message to be sent, 1-4096 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the message text. See formatting options for more details.
    [[optional]] entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in message text, which can be specified instead of parse_mode
    [[optional]] disable_web_page_preview : bool = false	//! Optional. Disables link previews for links in this message
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct formatting_options {} //  The Bot API supports basic formatting for messages. You can use bold, italic, underlined, strikethrough, and spoiler text, as well as inline links and pre-formatted code in your bots' messages. Telegram clients will render them accordingly. You can specify text entities directly, or use markdown-style or HTML-style formatting. Note that Telegram clients will display an alert to the user before opening an inline link ('Open this link?' together with the full URL). Message entities can be nested, providing following restrictions are met: - If two entities have common characters, then one of them is fully contained inside another. - bold, italic, underline, strikethrough, and spoiler entities can contain and can be part of any other entities, except pre and code. - All other entities can't contain each other. Links tg://user?id=<user_id> can be used to mention a user by their ID without using a username. Please note: To use this mode, pass MarkdownV2 in the parse_mode field. Use the following syntax in your message: Please note: To use this mode, pass HTML in the parse_mode field. The following tags are currently supported: Please note: This is a legacy mode, retained for backward compatibility. To use this mode, pass Markdown in the parse_mode field. Use the following syntax in your message: Please note:

struct forwardmessage
    //!  Use this method to forward messages of any kind. Service messages can't be forwarded. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    from_chat_id : string	//! Required. Unique identifier for the chat where the original message was sent (or channel username in the format @channelusername)
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the forwarded message from forwarding and saving
    message_id : int	//! Required. Message identifier in the chat specified in from_chat_id

struct copymessage
    //!  Use this method to copy messages of any kind. Service messages and invoice messages can't be copied. A quiz poll can be copied only if the value of the field correct_option_id is known to the bot. The method is analogous to the method forwardMessage, but the copied message doesn't have a link to the original message. Returns the MessageId of the sent message on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    from_chat_id : string	//! Required. Unique identifier for the chat where the original message was sent (or channel username in the format @channelusername)
    message_id : int	//! Required. Message identifier in the chat specified in from_chat_id
    [[optional]] caption : string = ""	//! Optional. New caption for media, 0-1024 characters after entities parsing. If not specified, the original caption is kept
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the new caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the new caption, which can be specified instead of parse_mode
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendphoto
    //!  Use this method to send photos. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    photo : inputfile	//! Required. Photo to send. Pass a file_id as String to send a photo that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a photo from the Internet, or upload a new photo using multipart/form-data. The photo must be at most 10 MB in size. The photo's width and height must not exceed 10000 in total. Width and height ratio must be at most 20. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Photo caption (may also be used when resending photos by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool = false	//! Optional. Pass True if the photo needs to be covered with a spoiler animation
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendaudio
    //!  Use this method to send audio files, if you want Telegram clients to display them in the music player. Your audio must be in the .MP3 or .M4A format. On success, the sent Message is returned. Bots can currently send audio files of up to 50 MB in size, this limit may be changed in the future. For sending voice messages, use the sendVoice method instead.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    audio : inputfile	//! Required. Audio file to send. Pass a file_id as String to send an audio file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get an audio file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Audio caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] duration : int = INT_MIN	//! Optional. Duration of the audio in seconds
    [[optional]] performer : string = ""	//! Optional. Performer
    [[optional]] title : string = ""	//! Optional. Track name
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct senddocument
    //!  Use this method to send general files. On success, the sent Message is returned. Bots can currently send files of any type of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    document : inputfile	//! Required. File to send. Pass a file_id as String to send a file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files »
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Document caption (may also be used when resending documents by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] disable_content_type_detection : bool = false	//! Optional. Disables automatic server-side content type detection for files uploaded using multipart/form-data
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendvideo
    //!  Use this method to send video files, Telegram clients support MPEG4 videos (other formats may be sent as Document). On success, the sent Message is returned. Bots can currently send video files of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    video : inputfile	//! Required. Video to send. Pass a file_id as String to send a video that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a video from the Internet, or upload a new video using multipart/form-data. More information on Sending Files »
    [[optional]] duration : int = INT_MIN	//! Optional. Duration of sent video in seconds
    [[optional]] width : int = INT_MIN	//! Optional. Video width
    [[optional]] height : int = INT_MIN	//! Optional. Video height
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Video caption (may also be used when resending videos by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool = false	//! Optional. Pass True if the video needs to be covered with a spoiler animation
    [[optional]] supports_streaming : bool = false	//! Optional. Pass True if the uploaded video is suitable for streaming
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendanimation
    //!  Use this method to send animation files (GIF or H.264/MPEG-4 AVC video without sound). On success, the sent Message is returned. Bots can currently send animation files of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    animation : inputfile	//! Required. Animation to send. Pass a file_id as String to send an animation that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get an animation from the Internet, or upload a new animation using multipart/form-data. More information on Sending Files »
    [[optional]] duration : int = INT_MIN	//! Optional. Duration of sent animation in seconds
    [[optional]] width : int = INT_MIN	//! Optional. Animation width
    [[optional]] height : int = INT_MIN	//! Optional. Animation height
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Animation caption (may also be used when resending animation by file_id), 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the animation caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] has_spoiler : bool = false	//! Optional. Pass True if the animation needs to be covered with a spoiler animation
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendvoice
    //!  Use this method to send audio files, if you want Telegram clients to display the file as a playable voice message. For this to work, your audio must be in an .OGG file encoded with OPUS (other formats may be sent as Audio or Document). On success, the sent Message is returned. Bots can currently send voice messages of up to 50 MB in size, this limit may be changed in the future.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    voice : inputfile	//! Required. Audio file to send. Pass a file_id as String to send a file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files »
    [[optional]] caption : string = ""	//! Optional. Voice message caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] duration : int = INT_MIN	//! Optional. Duration of the voice message in seconds
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendvideonote
    //!  As of v.4.0, Telegram clients support rounded square MPEG4 videos of up to 1 minute long. Use this method to send video messages. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    video_note : inputfile	//! Required. Video note to send. Pass a file_id as String to send a video note that exists on the Telegram servers (recommended) or upload a new video using multipart/form-data. More information on Sending Files ». Sending video notes by a URL is currently unsupported
    [[optional]] duration : int = INT_MIN	//! Optional. Duration of sent video in seconds
    [[optional]] length : int = INT_MIN	//! Optional. Video width and height, i.e. diameter of the video message
    [[optional]] thumbnail : inputfile?	//! Optional. Thumbnail of the file sent; can be ignored if thumbnail generation for the file is supported server-side. The thumbnail should be in JPEG format and less than 200 kB in size. A thumbnail's width and height should not exceed 320. Ignored if the file is not uploaded using multipart/form-data. Thumbnails can't be reused and can be only uploaded as a new file, so you can pass “attach://<file_attach_name>” if the thumbnail was uploaded using multipart/form-data under <file_attach_name>. More information on Sending Files »
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendmediagroup
    //!  Use this method to send a group of photos, videos, documents or audios as an album. Documents and audio files can be only grouped in an album with messages of the same type. On success, an array of Messages that were sent is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    media : array<inputmediaaudio>	//! Required. A JSON-serialized array describing messages to be sent, must include 2-10 items
    [[optional]] disable_notification : bool = false	//! Optional. Sends messages silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent messages from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the messages are a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found

struct sendlocation
    //!  Use this method to send point on the map. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    latitude : float	//! Required. Latitude of the location
    longitude : float	//! Required. Longitude of the location
    [[optional]] horizontal_accuracy : float = -FLT_MAX	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int = INT_MIN	//! Optional. Period in seconds for which the location will be updated (see Live Locations, should be between 60 and 86400.
    [[optional]] heading : int = INT_MIN	//! Optional. For live locations, a direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int = INT_MIN	//! Optional. For live locations, a maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendvenue
    //!  Use this method to send information about a venue. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    latitude : float	//! Required. Latitude of the venue
    longitude : float	//! Required. Longitude of the venue
    title : string	//! Required. Name of the venue
    address : string	//! Required. Address of the venue
    [[optional]] foursquare_id : string = ""	//! Optional. Foursquare identifier of the venue
    [[optional]] foursquare_type : string = ""	//! Optional. Foursquare type of the venue, if known. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string = ""	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string = ""	//! Optional. Google Places type of the venue. (See supported types.)
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendcontact
    //!  Use this method to send phone contacts. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    phone_number : string	//! Required. Contact's phone number
    first_name : string	//! Required. Contact's first name
    [[optional]] last_name : string = ""	//! Optional. Contact's last name
    [[optional]] vcard : string = ""	//! Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendpoll
    //!  Use this method to send a native poll. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    question : string	//! Required. Poll question, 1-300 characters
    _options : array<string>	//! Required. A JSON-serialized list of answer options, 2-10 strings 1-100 characters each
    [[optional]] is_anonymous : bool = false	//! Optional. True, if the poll needs to be anonymous, defaults to True
    [[optional]] _type : string = ""	//! Optional. Poll type, “quiz” or “regular”, defaults to “regular”
    [[optional]] allows_multiple_answers : bool = false	//! Optional. True, if the poll allows multiple answers, ignored for polls in quiz mode, defaults to False
    [[optional]] correct_option_id : int = INT_MIN	//! Optional. 0-based identifier of the correct answer option, required for polls in quiz mode
    [[optional]] explanation : string = ""	//! Optional. Text that is shown when a user chooses an incorrect answer or taps on the lamp icon in a quiz-style poll, 0-200 characters with at most 2 line feeds after entities parsing
    [[optional]] explanation_parse_mode : string = ""	//! Optional. Mode for parsing entities in the explanation. See formatting options for more details.
    [[optional]] explanation_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the poll explanation, which can be specified instead of parse_mode
    [[optional]] open_period : int = INT_MIN	//! Optional. Amount of time in seconds the poll will be active after creation, 5-600. Can't be used together with close_date.
    [[optional]] close_date : int = INT_MIN	//! Optional. Point in time (Unix timestamp) when the poll will be automatically closed. Must be at least 5 and no more than 600 seconds in the future. Can't be used together with open_period.
    [[optional]] is_closed : bool = false	//! Optional. Pass True if the poll needs to be immediately closed. This can be useful for poll preview.
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct senddice
    //!  Use this method to send an animated emoji that will display a random value. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    [[optional]] emoji : string = ""	//! Optional. Emoji on which the dice throw animation is based. Currently, must be one of “”, “”, “”, “”, “”, or “”. Dice can have values 1-6 for “”, “” and “”, values 1-5 for “” and “”, and values 1-64 for “”. Defaults to “”
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct sendchataction
    //!  Use this method when you need to tell the user that something is happening on the bot's side. The status is set for 5 seconds or less (when a message arrives from your bot, Telegram clients clear its typing status). Returns True on success. We only recommend using this method when a response from the bot will take a noticeable amount of time to arrive.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread; supergroups only
    action : string	//! Required. Type of action to broadcast. Choose one, depending on what the user is about to receive: typing for text messages, upload_photo for photos, record_video or upload_video for videos, record_voice or upload_voice for voice notes, upload_document for general files, choose_sticker for stickers, find_location for location data, record_video_note or upload_video_note for video notes.

struct getuserprofilephotos
    //!  Use this method to get a list of profile pictures for a user. Returns a UserProfilePhotos object.
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] offset : int = INT_MIN	//! Optional. Sequential number of the first photo to be returned. By default, all photos are returned.
    [[optional]] limit : int = INT_MIN	//! Optional. Limits the number of photos to be retrieved. Values between 1-100 are accepted. Defaults to 100.

struct getfile
    //!  Use this method to get basic information about a file and prepare it for downloading. For the moment, bots can download files of up to 20MB in size. On success, a File object is returned. The file can then be downloaded via the link https://api.telegram.org/file/bot<token>/<file_path>, where <file_path> is taken from the response. It is guaranteed that the link will be valid for at least 1 hour. When the link expires, a new one can be requested by calling getFile again.
    file_id : string	//! Required. File identifier to get information about

struct banchatmember
    //!  Use this method to ban a user in a group, a supergroup or a channel. In the case of supergroups and channels, the user will not be able to return to the chat on their own using invite links, etc., unless unbanned first. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target group or username of the target supergroup or channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] until_date : int = INT_MIN	//! Optional. Date when the user will be unbanned, unix time. If user is banned for more than 366 days or less than 30 seconds from the current time they are considered to be banned forever. Applied for supergroups and channels only.
    [[optional]] revoke_messages : bool = false	//! Optional. Pass True to delete all messages from the chat for the user that is being removed. If False, the user will be able to see messages in the group that were sent before the user was removed. Always True for supergroups and channels.

struct unbanchatmember
    //!  Use this method to unban a previously banned user in a supergroup or channel. The user will not return to the group or channel automatically, but will be able to join via link, etc. The bot must be an administrator for this to work. By default, this method guarantees that after the call the user is not a member of the chat, but will be able to join it. So if the user is a member of the chat they will also be removed from the chat. If you don't want this, use the parameter only_if_banned. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target group or username of the target supergroup or channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] only_if_banned : bool = false	//! Optional. Do nothing if the user is not banned

struct restrictchatmember
    //!  Use this method to restrict a user in a supergroup. The bot must be an administrator in the supergroup for this to work and must have the appropriate administrator rights. Pass True for all permissions to lift restrictions from a user. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    user_id : int	//! Required. Unique identifier of the target user
    permissions : chatpermissions	//! Required. A JSON-serialized object for new user permissions
    [[optional]] use_independent_chat_permissions : bool = false	//! Optional. Pass True if chat permissions are set independently. Otherwise, the can_send_other_messages and can_add_web_page_previews permissions will imply the can_send_messages, can_send_audios, can_send_documents, can_send_photos, can_send_videos, can_send_video_notes, and can_send_voice_notes permissions; the can_send_polls permission will imply the can_send_messages permission.
    [[optional]] until_date : int = INT_MIN	//! Optional. Date when restrictions will be lifted for the user, unix time. If user is restricted for more than 366 days or less than 30 seconds from the current time, they are considered to be restricted forever

struct promotechatmember
    //!  Use this method to promote or demote a user in a supergroup or a channel. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Pass False for all boolean parameters to demote a user. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user
    [[optional]] is_anonymous : bool = false	//! Optional. Pass True if the administrator's presence in the chat is hidden
    [[optional]] can_manage_chat : bool = false	//! Optional. Pass True if the administrator can access the chat event log, chat statistics, message statistics in channels, see channel members, see anonymous administrators in supergroups and ignore slow mode. Implied by any other administrator privilege
    [[optional]] can_post_messages : bool = false	//! Optional. Pass True if the administrator can create channel posts, channels only
    [[optional]] can_edit_messages : bool = false	//! Optional. Pass True if the administrator can edit messages of other users and can pin messages, channels only
    [[optional]] can_delete_messages : bool = false	//! Optional. Pass True if the administrator can delete messages of other users
    [[optional]] can_manage_video_chats : bool = false	//! Optional. Pass True if the administrator can manage video chats
    [[optional]] can_restrict_members : bool = false	//! Optional. Pass True if the administrator can restrict, ban or unban chat members
    [[optional]] can_promote_members : bool = false	//! Optional. Pass True if the administrator can add new administrators with a subset of their own privileges or demote administrators that they have promoted, directly or indirectly (promoted by administrators that were appointed by him)
    [[optional]] can_change_info : bool = false	//! Optional. Pass True if the administrator can change chat title, photo and other settings
    [[optional]] can_invite_users : bool = false	//! Optional. Pass True if the administrator can invite new users to the chat
    [[optional]] can_pin_messages : bool = false	//! Optional. Pass True if the administrator can pin messages, supergroups only
    [[optional]] can_manage_topics : bool = false	//! Optional. Pass True if the user is allowed to create, rename, close, and reopen forum topics, supergroups only

struct setchatadministratorcustomtitle
    //!  Use this method to set a custom title for an administrator in a supergroup promoted by the bot. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    user_id : int	//! Required. Unique identifier of the target user
    custom_title : string	//! Required. New custom title for the administrator; 0-16 characters, emoji are not allowed

struct banchatsenderchat
    //!  Use this method to ban a channel chat in a supergroup or a channel. Until the chat is unbanned, the owner of the banned chat won't be able to send messages on behalf of any of their channels. The bot must be an administrator in the supergroup or channel for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    sender_chat_id : int	//! Required. Unique identifier of the target sender chat

struct unbanchatsenderchat
    //!  Use this method to unban a previously banned channel chat in a supergroup or channel. The bot must be an administrator for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    sender_chat_id : int	//! Required. Unique identifier of the target sender chat

struct setchatpermissions
    //!  Use this method to set default chat permissions for all members. The bot must be an administrator in the group or a supergroup for this to work and must have the can_restrict_members administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    permissions : chatpermissions	//! Required. A JSON-serialized object for new default chat permissions
    [[optional]] use_independent_chat_permissions : bool = false	//! Optional. Pass True if chat permissions are set independently. Otherwise, the can_send_other_messages and can_add_web_page_previews permissions will imply the can_send_messages, can_send_audios, can_send_documents, can_send_photos, can_send_videos, can_send_video_notes, and can_send_voice_notes permissions; the can_send_polls permission will imply the can_send_messages permission.

struct exportchatinvitelink
    //!  Use this method to generate a new primary invite link for a chat; any previously generated primary link is revoked. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns the new invite link as String on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)

struct createchatinvitelink
    //!  Use this method to create an additional invite link for a chat. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. The link can be revoked using the method revokeChatInviteLink. Returns the new invite link as ChatInviteLink object.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] name : string = ""	//! Optional. Invite link name; 0-32 characters
    [[optional]] expire_date : int = INT_MIN	//! Optional. Point in time (Unix timestamp) when the link will expire
    [[optional]] member_limit : int = INT_MIN	//! Optional. The maximum number of users that can be members of the chat simultaneously after joining the chat via this invite link; 1-99999
    [[optional]] creates_join_request : bool = false	//! Optional. True, if users joining the chat via the link need to be approved by chat administrators. If True, member_limit can't be specified

struct editchatinvitelink
    //!  Use this method to edit a non-primary invite link created by the bot. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns the edited invite link as a ChatInviteLink object.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    invite_link : string	//! Required. The invite link to edit
    [[optional]] name : string = ""	//! Optional. Invite link name; 0-32 characters
    [[optional]] expire_date : int = INT_MIN	//! Optional. Point in time (Unix timestamp) when the link will expire
    [[optional]] member_limit : int = INT_MIN	//! Optional. The maximum number of users that can be members of the chat simultaneously after joining the chat via this invite link; 1-99999
    [[optional]] creates_join_request : bool = false	//! Optional. True, if users joining the chat via the link need to be approved by chat administrators. If True, member_limit can't be specified

struct revokechatinvitelink
    //!  Use this method to revoke an invite link created by the bot. If the primary link is revoked, a new link is automatically generated. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns the revoked invite link as ChatInviteLink object.
    chat_id : string	//! Required. Unique identifier of the target chat or username of the target channel (in the format @channelusername)
    invite_link : string	//! Required. The invite link to revoke

struct approvechatjoinrequest
    //!  Use this method to approve a chat join request. The bot must be an administrator in the chat for this to work and must have the can_invite_users administrator right. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user

struct declinechatjoinrequest
    //!  Use this method to decline a chat join request. The bot must be an administrator in the chat for this to work and must have the can_invite_users administrator right. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user

struct setchatphoto
    //!  Use this method to set a new profile photo for the chat. Photos can't be changed for private chats. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    photo : inputfile	//! Required. New chat photo, uploaded using multipart/form-data

struct deletechatphoto
    //!  Use this method to delete a chat photo. Photos can't be changed for private chats. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)

struct setchattitle
    //!  Use this method to change the title of a chat. Titles can't be changed for private chats. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    title : string	//! Required. New chat title, 1-128 characters

struct setchatdescription
    //!  Use this method to change the description of a group, a supergroup or a channel. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] description : string = ""	//! Optional. New chat description, 0-255 characters

struct pinchatmessage
    //!  Use this method to add a message to the list of pinned messages in a chat. If the chat is not a private chat, the bot must be an administrator in the chat for this to work and must have the 'can_pin_messages' administrator right in a supergroup or 'can_edit_messages' administrator right in a channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    message_id : int	//! Required. Identifier of a message to pin
    [[optional]] disable_notification : bool = false	//! Optional. Pass True if it is not necessary to send a notification to all chat members about the new pinned message. Notifications are always disabled in channels and private chats.

struct unpinchatmessage
    //!  Use this method to remove a message from the list of pinned messages in a chat. If the chat is not a private chat, the bot must be an administrator in the chat for this to work and must have the 'can_pin_messages' administrator right in a supergroup or 'can_edit_messages' administrator right in a channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Identifier of a message to unpin. If not specified, the most recent pinned message (by sending date) will be unpinned.

struct unpinallchatmessages
    //!  Use this method to clear the list of pinned messages in a chat. If the chat is not a private chat, the bot must be an administrator in the chat for this to work and must have the 'can_pin_messages' administrator right in a supergroup or 'can_edit_messages' administrator right in a channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)

struct leavechat
    //!  Use this method for your bot to leave a group, supergroup or channel. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

struct getchat
    //!  Use this method to get up to date information about the chat (current name of the user for one-on-one conversations, current username of a user, group or channel, etc.). Returns a Chat object on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

struct getchatadministrators
    //!  Use this method to get a list of administrators in a chat, which aren't bots. Returns an Array of ChatMember objects.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

struct getchatmembercount
    //!  Use this method to get the number of members in a chat. Returns Int on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)

struct getchatmember
    //!  Use this method to get information about a member of a chat. The method is only guaranteed to work for other users if the bot is an administrator in the chat. Returns a ChatMember object on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup or channel (in the format @channelusername)
    user_id : int	//! Required. Unique identifier of the target user

struct setchatstickerset
    //!  Use this method to set a new group sticker set for a supergroup. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Use the field can_set_sticker_set optionally returned in getChat requests to check if the bot can use this method. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    sticker_set_name : string	//! Required. Name of the sticker set to be set as the group sticker set

struct deletechatstickerset
    //!  Use this method to delete a group sticker set from a supergroup. The bot must be an administrator in the chat for this to work and must have the appropriate administrator rights. Use the field can_set_sticker_set optionally returned in getChat requests to check if the bot can use this method. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct getforumtopiciconstickers {} //  Use this method to get custom emoji stickers, which can be used as a forum topic icon by any user. Requires no parameters. Returns an Array of Sticker objects.

struct createforumtopic
    //!  Use this method to create a topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. Returns information about the created topic as a ForumTopic object.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    name : string	//! Required. Topic name, 1-128 characters
    [[optional]] icon_color : int = INT_MIN	//! Optional. Color of the topic icon in RGB format. Currently, must be one of 7322096 (0x6FB9F0), 16766590 (0xFFD67E), 13338331 (0xCB86DB), 9367192 (0x8EEE98), 16749490 (0xFF93B2), or 16478047 (0xFB6F5F)
    [[optional]] icon_custom_emoji_id : string = ""	//! Optional. Unique identifier of the custom emoji shown as the topic icon. Use getForumTopicIconStickers to get all allowed custom emoji identifiers.

struct editforumtopic
    //!  Use this method to edit name and icon of a topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have can_manage_topics administrator rights, unless it is the creator of the topic. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic
    [[optional]] name : string = ""	//! Optional. New topic name, 0-128 characters. If not specified or empty, the current name of the topic will be kept
    [[optional]] icon_custom_emoji_id : string = ""	//! Optional. New unique identifier of the custom emoji shown as the topic icon. Use getForumTopicIconStickers to get all allowed custom emoji identifiers. Pass an empty string to remove the icon. If not specified, the current icon will be kept

struct closeforumtopic
    //!  Use this method to close an open topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights, unless it is the creator of the topic. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

struct reopenforumtopic
    //!  Use this method to reopen a closed topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights, unless it is the creator of the topic. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

struct deleteforumtopic
    //!  Use this method to delete a forum topic along with all its messages in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_delete_messages administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

struct unpinallforumtopicmessages
    //!  Use this method to clear the list of pinned messages in a forum topic. The bot must be an administrator in the chat for this to work and must have the can_pin_messages administrator right in the supergroup. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    message_thread_id : int	//! Required. Unique identifier for the target message thread of the forum topic

struct editgeneralforumtopic
    //!  Use this method to edit the name of the 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have can_manage_topics administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)
    name : string	//! Required. New topic name, 1-128 characters

struct closegeneralforumtopic
    //!  Use this method to close an open 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct reopengeneralforumtopic
    //!  Use this method to reopen a closed 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. The topic will be automatically unhidden if it was hidden. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct hidegeneralforumtopic
    //!  Use this method to hide the 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. The topic will be automatically closed if it was open. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct unhidegeneralforumtopic
    //!  Use this method to unhide the 'General' topic in a forum supergroup chat. The bot must be an administrator in the chat for this to work and must have the can_manage_topics administrator rights. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target supergroup (in the format @supergroupusername)

struct answercallbackquery
    //!  Use this method to send answers to callback queries sent from inline keyboards. The answer will be displayed to the user as a notification at the top of the chat screen or as an alert. On success, True is returned.
    callback_query_id : string	//! Required. Unique identifier for the query to be answered
    [[optional]] text : string = ""	//! Optional. Text of the notification. If not specified, nothing will be shown to the user, 0-200 characters
    [[optional]] show_alert : bool = false	//! Optional. If True, an alert will be shown by the client instead of a notification at the top of the chat screen. Defaults to false.
    [[optional]] url : string = ""	//! Optional. URL that will be opened by the user's client. If you have created a Game and accepted the conditions via @BotFather, specify the URL that opens your game - note that this will only work if the query comes from a callback_game button. Otherwise, you may use links like t.me/your_bot?start=XXXX that open your bot with a parameter.
    [[optional]] cache_time : int = INT_MIN	//! Optional. The maximum amount of time in seconds that the result of the callback query may be cached client-side. Telegram apps will support caching starting in version 3.14. Defaults to 0.

struct setmycommands
    //!  Use this method to change the list of the bot's commands. See this manual for more details about bot commands. Returns True on success.
    commands : array<botcommand>	//! Required. A JSON-serialized list of bot commands to be set as the list of the bot's commands. At most 100 commands can be specified.
    [[optional]] scope : botcommandscope?	//! Optional. A JSON-serialized object, describing scope of users for which the commands are relevant. Defaults to BotCommandScopeDefault.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code. If empty, commands will be applied to all users from the given scope, for whose language there are no dedicated commands

struct deletemycommands
    //!  Use this method to delete the list of the bot's commands for the given scope and user language. After deletion, higher level commands will be shown to affected users. Returns True on success.
    [[optional]] scope : botcommandscope?	//! Optional. A JSON-serialized object, describing scope of users for which the commands are relevant. Defaults to BotCommandScopeDefault.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code. If empty, commands will be applied to all users from the given scope, for whose language there are no dedicated commands

struct getmycommands
    //!  Use this method to get the current list of the bot's commands for the given scope and user language. Returns an Array of BotCommand objects. If commands aren't set, an empty list is returned.
    [[optional]] scope : botcommandscope?	//! Optional. A JSON-serialized object, describing scope of users. Defaults to BotCommandScopeDefault.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code or an empty string

struct setmydescription
    //!  Use this method to change the bot's description, which is shown in the chat with the bot if the chat is empty. Returns True on success.
    [[optional]] description : string = ""	//! Optional. New bot description; 0-512 characters. Pass an empty string to remove the dedicated description for the given language.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code. If empty, the description will be applied to all users for whose language there is no dedicated description.

struct getmydescription
    //!  Use this method to get the current bot description for the given user language. Returns BotDescription on success.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code or an empty string

struct setmyshortdescription
    //!  Use this method to change the bot's short description, which is shown on the bot's profile page and is sent together with the link when users share the bot. Returns True on success.
    [[optional]] short_description : string = ""	//! Optional. New short description for the bot; 0-120 characters. Pass an empty string to remove the dedicated short description for the given language.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code. If empty, the short description will be applied to all users for whose language there is no dedicated short description.

struct getmyshortdescription
    //!  Use this method to get the current bot short description for the given user language. Returns BotShortDescription on success.
    [[optional]] language_code : string = ""	//! Optional. A two-letter ISO 639-1 language code or an empty string

struct setchatmenubutton
    //!  Use this method to change the bot's menu button in a private chat, or the default menu button. Returns True on success.
    [[optional]] chat_id : int = INT_MIN	//! Optional. Unique identifier for the target private chat. If not specified, default bot's menu button will be changed
    [[optional]] menu_button : menubutton?	//! Optional. A JSON-serialized object for the bot's new menu button. Defaults to MenuButtonDefault

struct getchatmenubutton
    //!  Use this method to get the current value of the bot's menu button in a private chat, or the default menu button. Returns MenuButton on success.
    [[optional]] chat_id : int = INT_MIN	//! Optional. Unique identifier for the target private chat. If not specified, default bot's menu button will be returned

struct setmydefaultadministratorrights
    //!  Use this method to change the default administrator rights requested by the bot when it's added as an administrator to groups or channels. These rights will be suggested to users, but they are free to modify the list before adding the bot. Returns True on success.
    [[optional]] rights : chatadministratorrights?	//! Optional. A JSON-serialized object describing new default administrator rights. If not specified, the default administrator rights will be cleared.
    [[optional]] for_channels : bool = false	//! Optional. Pass True to change the default administrator rights of the bot in channels. Otherwise, the default administrator rights of the bot for groups and supergroups will be changed.

struct getmydefaultadministratorrights
    //!  Use this method to get the current default administrator rights of the bot. Returns ChatAdministratorRights on success.
    [[optional]] for_channels : bool = false	//! Optional. Pass True to get default administrator rights of the bot in channels. Otherwise, default administrator rights of the bot for groups and supergroups will be returned.

struct inline_mode_methods {} //  Methods and objects used in the inline mode are described in the Inline mode section.

struct editmessagetext
    //!  Use this method to edit text and game messages. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string = ""	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    text : string	//! Required. New text of the message, 1-4096 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the message text. See formatting options for more details.
    [[optional]] entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in message text, which can be specified instead of parse_mode
    [[optional]] disable_web_page_preview : bool = false	//! Optional. Disables link previews for links in this message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard.

struct editmessagecaption
    //!  Use this method to edit captions of messages. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string = ""	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    [[optional]] caption : string = ""	//! Optional. New caption of the message, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. A JSON-serialized list of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard.

struct editmessagemedia
    //!  Use this method to edit animation, audio, document, photo, or video messages. If a message is part of a message album, then it can be edited only to an audio for audio albums, only to a document for document albums and to a photo or a video otherwise. When an inline message is edited, a new file can't be uploaded; use a previously uploaded file via its file_id or specify a URL. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string = ""	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    media : inputmedia	//! Required. A JSON-serialized object for a new media content of the message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new inline keyboard.

struct editmessagelivelocation
    //!  Use this method to edit live location messages. A location can be edited until its live_period expires or editing is explicitly disabled by a call to stopMessageLiveLocation. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string = ""	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    latitude : float	//! Required. Latitude of new location
    longitude : float	//! Required. Longitude of new location
    [[optional]] horizontal_accuracy : float = -FLT_MAX	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] heading : int = INT_MIN	//! Optional. Direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int = INT_MIN	//! Optional. The maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new inline keyboard.

struct stopmessagelivelocation
    //!  Use this method to stop updating a live location message before live_period expires. On success, if the message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string = ""	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the message with live location to stop
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new inline keyboard.

struct editmessagereplymarkup
    //!  Use this method to edit only the reply markup of messages. On success, if the edited message is not an inline message, the edited Message is returned, otherwise True is returned.
    [[optional]] chat_id : string = ""	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the message to edit
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard.

struct stoppoll
    //!  Use this method to stop a poll which was sent by the bot. On success, the stopped Poll is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    message_id : int	//! Required. Identifier of the original message with the poll
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for a new message inline keyboard.

struct deletemessage
    //!  Use this method to delete a message, including service messages, with the following limitations: - A message can only be deleted if it was sent less than 48 hours ago. - Service messages about a supergroup, channel, or forum topic creation can't be deleted. - A dice message in a private chat can only be deleted if it was sent more than 24 hours ago. - Bots can delete outgoing messages in private chats, groups, and supergroups. - Bots can delete incoming messages in private chats. - Bots granted can_post_messages permissions can delete outgoing messages in channels. - If the bot is an administrator of a group, it can delete any message there. - If the bot has can_delete_messages permission in a supergroup or a channel, it can delete any message there. Returns True on success.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    message_id : int	//! Required. Identifier of the message to delete

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
    [[optional]] emoji : string = ""	//! Optional. Emoji associated with the sticker
    [[optional]] set_name : string = ""	//! Optional. Name of the sticker set to which the sticker belongs
    [[optional]] premium_animation : file?	//! Optional. For premium regular stickers, premium animation for the sticker
    [[optional]] mask_position : maskposition?	//! Optional. For mask stickers, the position where the mask should be placed
    [[optional]] custom_emoji_id : string = ""	//! Optional. For custom emoji stickers, unique identifier of the custom emoji
    [[optional]] needs_repainting : bool = false	//! Optional. True, if the sticker must be repainted to a text color in messages, the color of the Telegram Premium badge in emoji status, white color on chat photos, or another appropriate color in other places
    [[optional]] file_size : int = INT_MIN	//! Optional. File size in bytes

struct stickerset
    //!  This object represents a sticker set.
    name : string	//! Sticker set name
    title : string	//! Sticker set title
    sticker_type : string	//! Type of stickers in the set, currently one of “regular”, “mask”, “custom_emoji”
    is_animated : bool	//! True, if the sticker set contains animated stickers
    is_video : bool	//! True, if the sticker set contains video stickers
    stickers : array<sticker>	//! List of all set stickers
    [[optional]] thumbnail : photosize?	//! Optional. Sticker set thumbnail in the .WEBP, .TGS, or .WEBM format

struct maskposition
    //!  This object describes the position on faces where a mask should be placed by default.
    point : string	//! The part of the face relative to which the mask should be placed. One of “forehead”, “eyes”, “mouth”, or “chin”.
    x_shift : float	//! Shift by X-axis measured in widths of the mask scaled to the face size, from left to right. For example, choosing -1.0 will place mask just to the left of the default mask position.
    y_shift : float	//! Shift by Y-axis measured in heights of the mask scaled to the face size, from top to bottom. For example, 1.0 will place the mask just below the default mask position.
    scale : float	//! Mask scaling coefficient. For example, 2.0 means double size.

struct inputsticker
    //!  This object describes a sticker to be added to a sticker set.
    sticker : inputfile	//! The added sticker. Pass a file_id as a String to send a file that already exists on the Telegram servers, pass an HTTP URL as a String for Telegram to get a file from the Internet, upload a new one using multipart/form-data, or pass “attach://<file_attach_name>” to upload a new one using multipart/form-data under <file_attach_name> name. Animated and video stickers can't be uploaded via HTTP URL. More information on Sending Files »
    emoji_list : array<string>	//! List of 1-20 emoji associated with the sticker
    [[optional]] mask_position : maskposition?	//! Optional. Position where the mask should be placed on faces. For “mask” stickers only.
    [[optional]] keywords : array<string>	//! Optional. List of 0-20 search keywords for the sticker with total length of up to 64 characters. For “regular” and “custom_emoji” stickers only.

struct sendsticker
    //!  Use this method to send static .WEBP, animated .TGS, or video .WEBM stickers. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    sticker : inputfile	//! Required. Sticker to send. Pass a file_id as String to send a file that exists on the Telegram servers (recommended), pass an HTTP URL as a String for Telegram to get a .WEBP sticker from the Internet, or upload a new .WEBP or .TGS sticker using multipart/form-data. More information on Sending Files ». Video stickers can only be sent by a file_id. Animated stickers can't be sent via an HTTP URL.
    [[optional]] emoji : string = ""	//! Optional. Emoji associated with the sticker; only for just uploaded stickers
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Additional interface options. A JSON-serialized object for an inline keyboard, custom reply keyboard, instructions to remove reply keyboard or to force a reply from the user.

struct getstickerset
    //!  Use this method to get a sticker set. On success, a StickerSet object is returned.
    name : string	//! Required. Name of the sticker set

struct getcustomemojistickers
    //!  Use this method to get information about custom emoji stickers by their identifiers. Returns an Array of Sticker objects.
    custom_emoji_ids : array<string>	//! Required. List of custom emoji identifiers. At most 200 custom emoji identifiers can be specified.

struct uploadstickerfile
    //!  Use this method to upload a file with a sticker for later use in the createNewStickerSet and addStickerToSet methods (the file can be used multiple times). Returns the uploaded File on success.
    user_id : int	//! Required. User identifier of sticker file owner
    sticker : inputfile	//! Required. A file with the sticker in .WEBP, .PNG, .TGS, or .WEBM format. See https://core.telegram.org/stickers for technical requirements. More information on Sending Files »
    sticker_format : string	//! Required. Format of the sticker, must be one of “static”, “animated”, “video”

struct createnewstickerset
    //!  Use this method to create a new sticker set owned by a user. The bot will be able to edit the sticker set thus created. Returns True on success.
    user_id : int	//! Required. User identifier of created sticker set owner
    name : string	//! Required. Short name of sticker set, to be used in t.me/addstickers/ URLs (e.g., animals). Can contain only English letters, digits and underscores. Must begin with a letter, can't contain consecutive underscores and must end in "_by_<bot_username>". <bot_username> is case insensitive. 1-64 characters.
    title : string	//! Required. Sticker set title, 1-64 characters
    stickers : array<inputsticker>	//! Required. A JSON-serialized list of 1-50 initial stickers to be added to the sticker set
    sticker_format : string	//! Required. Format of stickers in the set, must be one of “static”, “animated”, “video”
    [[optional]] sticker_type : string = ""	//! Optional. Type of stickers in the set, pass “regular”, “mask”, or “custom_emoji”. By default, a regular sticker set is created.
    [[optional]] needs_repainting : bool = false	//! Optional. Pass True if stickers in the sticker set must be repainted to the color of text when used in messages, the accent color if used as emoji status, white on chat photos, or another appropriate color based on context; for custom emoji sticker sets only

struct addstickertoset
    //!  Use this method to add a new sticker to a set created by the bot. The format of the added sticker must match the format of the other stickers in the set. Emoji sticker sets can have up to 200 stickers. Animated and video sticker sets can have up to 50 stickers. Static sticker sets can have up to 120 stickers. Returns True on success.
    user_id : int	//! Required. User identifier of sticker set owner
    name : string	//! Required. Sticker set name
    sticker : inputsticker	//! Required. A JSON-serialized object with information about the added sticker. If exactly the same sticker had already been added to the set, then the set isn't changed.

struct setstickerpositioninset
    //!  Use this method to move a sticker in a set created by the bot to a specific position. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    position : int	//! Required. New sticker position in the set, zero-based

struct deletestickerfromset
    //!  Use this method to delete a sticker from a set created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker

struct setstickeremojilist
    //!  Use this method to change the list of emoji assigned to a regular or custom emoji sticker. The sticker must belong to a sticker set created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    emoji_list : array<string>	//! Required. A JSON-serialized list of 1-20 emoji associated with the sticker

struct setstickerkeywords
    //!  Use this method to change search keywords assigned to a regular or custom emoji sticker. The sticker must belong to a sticker set created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    [[optional]] keywords : array<string>	//! Optional. A JSON-serialized list of 0-20 search keywords for the sticker with total length of up to 64 characters

struct setstickermaskposition
    //!  Use this method to change the mask position of a mask sticker. The sticker must belong to a sticker set that was created by the bot. Returns True on success.
    sticker : string	//! Required. File identifier of the sticker
    [[optional]] mask_position : maskposition?	//! Optional. A JSON-serialized object with the position where the mask should be placed on faces. Omit the parameter to remove the mask position.

struct setstickersettitle
    //!  Use this method to set the title of a created sticker set. Returns True on success.
    name : string	//! Required. Sticker set name
    title : string	//! Required. Sticker set title, 1-64 characters

struct setstickersetthumbnail
    //!  Use this method to set the thumbnail of a regular or mask sticker set. The format of the thumbnail file must match the format of the stickers in the set. Returns True on success.
    name : string	//! Required. Sticker set name
    user_id : int	//! Required. User identifier of the sticker set owner
    [[optional]] thumbnail : inputfile?	//! Optional. A .WEBP or .PNG image with the thumbnail, must be up to 128 kilobytes in size and have a width and height of exactly 100px, or a .TGS animation with a thumbnail up to 32 kilobytes in size (see https://core.telegram.org/stickers#animated-sticker-requirements for animated sticker technical requirements), or a WEBM video with the thumbnail up to 32 kilobytes in size; see https://core.telegram.org/stickers#video-sticker-requirements for video sticker technical requirements. Pass a file_id as a String to send a file that already exists on the Telegram servers, pass an HTTP URL as a String for Telegram to get a file from the Internet, or upload a new one using multipart/form-data. More information on Sending Files ». Animated and video sticker set thumbnails can't be uploaded via HTTP URL. If omitted, then the thumbnail is dropped and the first sticker is used as the thumbnail.

struct setcustomemojistickersetthumbnail
    //!  Use this method to set the thumbnail of a custom emoji sticker set. Returns True on success.
    name : string	//! Required. Sticker set name
    [[optional]] custom_emoji_id : string = ""	//! Optional. Custom emoji identifier of a sticker from the sticker set; pass an empty string to drop the thumbnail and use the first sticker as the thumbnail.

struct deletestickerset
    //!  Use this method to delete a sticker set that was created by the bot. Returns True on success.
    name : string	//! Required. Sticker set name

struct inlinequery
    //!  This object represents an incoming inline query. When the user sends an empty query, your bot could return some default or trending results.
    id : string	//! Unique identifier for this query
    from : user	//! Sender
    query : string	//! Text of the query (up to 256 characters)
    offset : string	//! Offset of the results to be returned, can be controlled by the bot
    [[optional]] chat_type : string = ""	//! Optional. Type of the chat from which the inline query was sent. Can be either “sender” for a private chat with the inline query sender, “private”, “group”, “supergroup”, or “channel”. The chat type should be always known for requests sent from official clients and most third-party clients, unless the request was sent from a secret chat
    [[optional]] location : location?	//! Optional. Sender location, only for bots that request user location

struct answerinlinequery
    //!  Use this method to send answers to an inline query. On success, True is returned. No more than 50 results per query are allowed.
    inline_query_id : string	//! Required. Unique identifier for the answered query
    results : array<inlinequeryresult>	//! Required. A JSON-serialized array of results for the inline query
    [[optional]] cache_time : int = INT_MIN	//! Optional. The maximum amount of time in seconds that the result of the inline query may be cached on the server. Defaults to 300.
    [[optional]] is_personal : bool = false	//! Optional. Pass True if results may be cached on the server side only for the user that sent the query. By default, results may be returned to any user who sends the same query
    [[optional]] next_offset : string = ""	//! Optional. Pass the offset that a client should send in the next query with the same text to receive more results. Pass an empty string if there are no more results or if you don't support pagination. Offset length can't exceed 64 bytes.
    [[optional]] switch_pm_text : string = ""	//! Optional. If passed, clients will display a button with specified text that switches the user to a private chat with the bot and sends the bot a start message with the parameter switch_pm_parameter
    [[optional]] switch_pm_parameter : string = ""	//! Optional. Deep-linking parameter for the /start message sent to the bot when user presses the switch button. 1-64 characters, only A-Z, a-z, 0-9, _ and - are allowed.Example: An inline bot that sends YouTube videos can ask the user to connect the bot to their YouTube account to adapt search results accordingly. To do this, it displays a 'Connect your YouTube account' button above the results, or even before showing any. The user presses the button, switches to a private chat with the bot and, in doing so, passes a start parameter that instructs the bot to return an OAuth link. Once done, the bot can offer a switch_inline button so that the user can easily return to the chat where they wanted to use the bot's inline capabilities.

struct inlinequeryresult {} //  This object represents one result of an inline query. Telegram clients currently support results of the following 20 types: Note: All URLs passed in inline query results will be available to end users and therefore must be assumed to be public.

struct inlinequeryresultarticle
    //!  Represents a link to an article or web page.
    _type : string	//! Type of the result, must be article
    id : string	//! Unique identifier for this result, 1-64 Bytes
    title : string	//! Title of the result
    input_message_content : inputmessagecontent	//! Content of the message to be sent
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] url : string = ""	//! Optional. URL of the result
    [[optional]] hide_url : bool = false	//! Optional. Pass True if you don't want the URL to be shown in the message
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] thumbnail_url : string = ""	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int = INT_MIN	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int = INT_MIN	//! Optional. Thumbnail height

struct inlinequeryresultphoto
    //!  Represents a link to a photo. By default, this photo will be sent by the user with optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the photo.
    _type : string	//! Type of the result, must be photo
    id : string	//! Unique identifier for this result, 1-64 bytes
    photo_url : string	//! A valid URL of the photo. Photo must be in JPEG format. Photo size must not exceed 5MB
    thumbnail_url : string	//! URL of the thumbnail for the photo
    [[optional]] photo_width : int = INT_MIN	//! Optional. Width of the photo
    [[optional]] photo_height : int = INT_MIN	//! Optional. Height of the photo
    [[optional]] title : string = ""	//! Optional. Title for the result
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] caption : string = ""	//! Optional. Caption of the photo to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the photo

struct inlinequeryresultgif
    //!  Represents a link to an animated GIF file. By default, this animated GIF file will be sent by the user with optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the animation.
    _type : string	//! Type of the result, must be gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    gif_url : string	//! A valid URL for the GIF file. File size must not exceed 1MB
    [[optional]] gif_width : int = INT_MIN	//! Optional. Width of the GIF
    [[optional]] gif_height : int = INT_MIN	//! Optional. Height of the GIF
    [[optional]] gif_duration : int = INT_MIN	//! Optional. Duration of the GIF in seconds
    thumbnail_url : string	//! URL of the static (JPEG or GIF) or animated (MPEG4) thumbnail for the result
    [[optional]] thumbnail_mime_type : string = ""	//! Optional. MIME type of the thumbnail, must be one of “image/jpeg”, “image/gif”, or “video/mp4”. Defaults to “image/jpeg”
    [[optional]] title : string = ""	//! Optional. Title for the result
    [[optional]] caption : string = ""	//! Optional. Caption of the GIF file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the GIF animation

struct inlinequeryresultmpeg4gif
    //!  Represents a link to a video animation (H.264/MPEG-4 AVC video without sound). By default, this animated MPEG-4 file will be sent by the user with optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the animation.
    _type : string	//! Type of the result, must be mpeg4_gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    mpeg4_url : string	//! A valid URL for the MPEG4 file. File size must not exceed 1MB
    [[optional]] mpeg4_width : int = INT_MIN	//! Optional. Video width
    [[optional]] mpeg4_height : int = INT_MIN	//! Optional. Video height
    [[optional]] mpeg4_duration : int = INT_MIN	//! Optional. Video duration in seconds
    thumbnail_url : string	//! URL of the static (JPEG or GIF) or animated (MPEG4) thumbnail for the result
    [[optional]] thumbnail_mime_type : string = ""	//! Optional. MIME type of the thumbnail, must be one of “image/jpeg”, “image/gif”, or “video/mp4”. Defaults to “image/jpeg”
    [[optional]] title : string = ""	//! Optional. Title for the result
    [[optional]] caption : string = ""	//! Optional. Caption of the MPEG-4 file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video animation

struct inlinequeryresultvideo
    //!  Represents a link to a page containing an embedded video player or a video file. By default, this video file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the video.
    _type : string	//! Type of the result, must be video
    id : string	//! Unique identifier for this result, 1-64 bytes
    video_url : string	//! A valid URL for the embedded video player or video file
    mime_type : string	//! MIME type of the content of the video URL, “text/html” or “video/mp4”
    thumbnail_url : string	//! URL of the thumbnail (JPEG only) for the video
    title : string	//! Title for the result
    [[optional]] caption : string = ""	//! Optional. Caption of the video to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] video_width : int = INT_MIN	//! Optional. Video width
    [[optional]] video_height : int = INT_MIN	//! Optional. Video height
    [[optional]] video_duration : int = INT_MIN	//! Optional. Video duration in seconds
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video. This field is required if InlineQueryResultVideo is used to send an HTML-page as a result (e.g., a YouTube video).

struct inlinequeryresultaudio
    //!  Represents a link to an MP3 audio file. By default, this audio file will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the audio.
    _type : string	//! Type of the result, must be audio
    id : string	//! Unique identifier for this result, 1-64 bytes
    audio_url : string	//! A valid URL for the audio file
    title : string	//! Title
    [[optional]] caption : string = ""	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] performer : string = ""	//! Optional. Performer
    [[optional]] audio_duration : int = INT_MIN	//! Optional. Audio duration in seconds
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the audio

struct inlinequeryresultvoice
    //!  Represents a link to a voice recording in an .OGG container encoded with OPUS. By default, this voice recording will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the the voice message.
    _type : string	//! Type of the result, must be voice
    id : string	//! Unique identifier for this result, 1-64 bytes
    voice_url : string	//! A valid URL for the voice recording
    title : string	//! Recording title
    [[optional]] caption : string = ""	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] voice_duration : int = INT_MIN	//! Optional. Recording duration in seconds
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the voice recording

struct inlinequeryresultdocument
    //!  Represents a link to a file. By default, this file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the file. Currently, only .PDF and .ZIP files can be sent using this method.
    _type : string	//! Type of the result, must be document
    id : string	//! Unique identifier for this result, 1-64 bytes
    title : string	//! Title for the result
    [[optional]] caption : string = ""	//! Optional. Caption of the document to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    document_url : string	//! A valid URL for the file
    mime_type : string	//! MIME type of the content of the file, either “application/pdf” or “application/zip”
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the file
    [[optional]] thumbnail_url : string = ""	//! Optional. URL of the thumbnail (JPEG only) for the file
    [[optional]] thumbnail_width : int = INT_MIN	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int = INT_MIN	//! Optional. Thumbnail height

struct inlinequeryresultlocation
    //!  Represents a location on a map. By default, the location will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the location.
    _type : string	//! Type of the result, must be location
    id : string	//! Unique identifier for this result, 1-64 Bytes
    latitude : float	//! Location latitude in degrees
    longitude : float	//! Location longitude in degrees
    title : string	//! Location title
    [[optional]] horizontal_accuracy : float = -FLT_MAX	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int = INT_MIN	//! Optional. Period in seconds for which the location can be updated, should be between 60 and 86400.
    [[optional]] heading : int = INT_MIN	//! Optional. For live locations, a direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int = INT_MIN	//! Optional. For live locations, a maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the location
    [[optional]] thumbnail_url : string = ""	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int = INT_MIN	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int = INT_MIN	//! Optional. Thumbnail height

struct inlinequeryresultvenue
    //!  Represents a venue. By default, the venue will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the venue.
    _type : string	//! Type of the result, must be venue
    id : string	//! Unique identifier for this result, 1-64 Bytes
    latitude : float	//! Latitude of the venue location in degrees
    longitude : float	//! Longitude of the venue location in degrees
    title : string	//! Title of the venue
    address : string	//! Address of the venue
    [[optional]] foursquare_id : string = ""	//! Optional. Foursquare identifier of the venue if known
    [[optional]] foursquare_type : string = ""	//! Optional. Foursquare type of the venue, if known. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string = ""	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string = ""	//! Optional. Google Places type of the venue. (See supported types.)
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the venue
    [[optional]] thumbnail_url : string = ""	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int = INT_MIN	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int = INT_MIN	//! Optional. Thumbnail height

struct inlinequeryresultcontact
    //!  Represents a contact with a phone number. By default, this contact will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the contact.
    _type : string	//! Type of the result, must be contact
    id : string	//! Unique identifier for this result, 1-64 Bytes
    phone_number : string	//! Contact's phone number
    first_name : string	//! Contact's first name
    [[optional]] last_name : string = ""	//! Optional. Contact's last name
    [[optional]] vcard : string = ""	//! Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the contact
    [[optional]] thumbnail_url : string = ""	//! Optional. Url of the thumbnail for the result
    [[optional]] thumbnail_width : int = INT_MIN	//! Optional. Thumbnail width
    [[optional]] thumbnail_height : int = INT_MIN	//! Optional. Thumbnail height

struct inlinequeryresultgame
    //!  Represents a Game.
    _type : string	//! Type of the result, must be game
    id : string	//! Unique identifier for this result, 1-64 bytes
    game_short_name : string	//! Short name of the game
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message

struct inlinequeryresultcachedphoto
    //!  Represents a link to a photo stored on the Telegram servers. By default, this photo will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the photo.
    _type : string	//! Type of the result, must be photo
    id : string	//! Unique identifier for this result, 1-64 bytes
    photo_file_id : string	//! A valid file identifier of the photo
    [[optional]] title : string = ""	//! Optional. Title for the result
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] caption : string = ""	//! Optional. Caption of the photo to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the photo caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the photo

struct inlinequeryresultcachedgif
    //!  Represents a link to an animated GIF file stored on the Telegram servers. By default, this animated GIF file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with specified content instead of the animation.
    _type : string	//! Type of the result, must be gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    gif_file_id : string	//! A valid file identifier for the GIF file
    [[optional]] title : string = ""	//! Optional. Title for the result
    [[optional]] caption : string = ""	//! Optional. Caption of the GIF file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the GIF animation

struct inlinequeryresultcachedmpeg4gif
    //!  Represents a link to a video animation (H.264/MPEG-4 AVC video without sound) stored on the Telegram servers. By default, this animated MPEG-4 file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the animation.
    _type : string	//! Type of the result, must be mpeg4_gif
    id : string	//! Unique identifier for this result, 1-64 bytes
    mpeg4_file_id : string	//! A valid file identifier for the MPEG4 file
    [[optional]] title : string = ""	//! Optional. Title for the result
    [[optional]] caption : string = ""	//! Optional. Caption of the MPEG-4 file to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video animation

struct inlinequeryresultcachedsticker
    //!  Represents a link to a sticker stored on the Telegram servers. By default, this sticker will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the sticker.
    _type : string	//! Type of the result, must be sticker
    id : string	//! Unique identifier for this result, 1-64 bytes
    sticker_file_id : string	//! A valid file identifier of the sticker
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the sticker

struct inlinequeryresultcacheddocument
    //!  Represents a link to a file stored on the Telegram servers. By default, this file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the file.
    _type : string	//! Type of the result, must be document
    id : string	//! Unique identifier for this result, 1-64 bytes
    title : string	//! Title for the result
    document_file_id : string	//! A valid file identifier for the file
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] caption : string = ""	//! Optional. Caption of the document to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the document caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the file

struct inlinequeryresultcachedvideo
    //!  Represents a link to a video file stored on the Telegram servers. By default, this video file will be sent by the user with an optional caption. Alternatively, you can use input_message_content to send a message with the specified content instead of the video.
    _type : string	//! Type of the result, must be video
    id : string	//! Unique identifier for this result, 1-64 bytes
    video_file_id : string	//! A valid file identifier for the video file
    title : string	//! Title for the result
    [[optional]] description : string = ""	//! Optional. Short description of the result
    [[optional]] caption : string = ""	//! Optional. Caption of the video to be sent, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the video caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the video

struct inlinequeryresultcachedvoice
    //!  Represents a link to a voice message stored on the Telegram servers. By default, this voice message will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the voice message.
    _type : string	//! Type of the result, must be voice
    id : string	//! Unique identifier for this result, 1-64 bytes
    voice_file_id : string	//! A valid file identifier for the voice message
    title : string	//! Voice message title
    [[optional]] caption : string = ""	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the voice message caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the voice message

struct inlinequeryresultcachedaudio
    //!  Represents a link to an MP3 audio file stored on the Telegram servers. By default, this audio file will be sent by the user. Alternatively, you can use input_message_content to send a message with the specified content instead of the audio.
    _type : string	//! Type of the result, must be audio
    id : string	//! Unique identifier for this result, 1-64 bytes
    audio_file_id : string	//! A valid file identifier for the audio file
    [[optional]] caption : string = ""	//! Optional. Caption, 0-1024 characters after entities parsing
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the audio caption. See formatting options for more details.
    [[optional]] caption_entities : array<messageentity>	//! Optional. List of special entities that appear in the caption, which can be specified instead of parse_mode
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. Inline keyboard attached to the message
    [[optional]] input_message_content : inputmessagecontent?	//! Optional. Content of the message to be sent instead of the audio

struct inputmessagecontent {} //  This object represents the content of a message to be sent as a result of an inline query. Telegram clients currently support the following 5 types:

struct inputtextmessagecontent
    //!  Represents the content of a text message to be sent as the result of an inline query.
    message_text : string	//! Text of the message to be sent, 1-4096 characters
    [[optional]] parse_mode : string = ""	//! Optional. Mode for parsing entities in the message text. See formatting options for more details.
    [[optional]] entities : array<messageentity>	//! Optional. List of special entities that appear in message text, which can be specified instead of parse_mode
    [[optional]] disable_web_page_preview : bool = false	//! Optional. Disables link previews for links in the sent message

struct inputlocationmessagecontent
    //!  Represents the content of a location message to be sent as the result of an inline query.
    latitude : float	//! Latitude of the location in degrees
    longitude : float	//! Longitude of the location in degrees
    [[optional]] horizontal_accuracy : float = -FLT_MAX	//! Optional. The radius of uncertainty for the location, measured in meters; 0-1500
    [[optional]] live_period : int = INT_MIN	//! Optional. Period in seconds for which the location can be updated, should be between 60 and 86400.
    [[optional]] heading : int = INT_MIN	//! Optional. For live locations, a direction in which the user is moving, in degrees. Must be between 1 and 360 if specified.
    [[optional]] proximity_alert_radius : int = INT_MIN	//! Optional. For live locations, a maximum distance for proximity alerts about approaching another chat member, in meters. Must be between 1 and 100000 if specified.

struct inputvenuemessagecontent
    //!  Represents the content of a venue message to be sent as the result of an inline query.
    latitude : float	//! Latitude of the venue in degrees
    longitude : float	//! Longitude of the venue in degrees
    title : string	//! Name of the venue
    address : string	//! Address of the venue
    [[optional]] foursquare_id : string = ""	//! Optional. Foursquare identifier of the venue, if known
    [[optional]] foursquare_type : string = ""	//! Optional. Foursquare type of the venue, if known. (For example, “arts_entertainment/default”, “arts_entertainment/aquarium” or “food/icecream”.)
    [[optional]] google_place_id : string = ""	//! Optional. Google Places identifier of the venue
    [[optional]] google_place_type : string = ""	//! Optional. Google Places type of the venue. (See supported types.)

struct inputcontactmessagecontent
    //!  Represents the content of a contact message to be sent as the result of an inline query.
    phone_number : string	//! Contact's phone number
    first_name : string	//! Contact's first name
    [[optional]] last_name : string = ""	//! Optional. Contact's last name
    [[optional]] vcard : string = ""	//! Optional. Additional data about the contact in the form of a vCard, 0-2048 bytes

struct inputinvoicemessagecontent
    //!  Represents the content of an invoice message to be sent as the result of an inline query.
    title : string	//! Product name, 1-32 characters
    description : string	//! Product description, 1-255 characters
    payload : string	//! Bot-defined invoice payload, 1-128 bytes. This will not be displayed to the user, use for your internal processes.
    provider_token : string	//! Payment provider token, obtained via @BotFather
    currency : string	//! Three-letter ISO 4217 currency code, see more on currencies
    prices : array<labeledprice>	//! Price breakdown, a JSON-serialized list of components (e.g. product price, tax, discount, delivery cost, delivery tax, bonus, etc.)
    [[optional]] max_tip_amount : int = INT_MIN	//! Optional. The maximum accepted amount for tips in the smallest units of the currency (integer, not float/double). For example, for a maximum tip of US$ 1.45 pass max_tip_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies). Defaults to 0
    [[optional]] suggested_tip_amounts : array<int>	//! Optional. A JSON-serialized array of suggested amounts of tip in the smallest units of the currency (integer, not float/double). At most 4 suggested tip amounts can be specified. The suggested tip amounts must be positive, passed in a strictly increased order and must not exceed max_tip_amount.
    [[optional]] provider_data : string = ""	//! Optional. A JSON-serialized object for data about the invoice, which will be shared with the payment provider. A detailed description of the required fields should be provided by the payment provider.
    [[optional]] photo_url : string = ""	//! Optional. URL of the product photo for the invoice. Can be a photo of the goods or a marketing image for a service.
    [[optional]] photo_size : int = INT_MIN	//! Optional. Photo size in bytes
    [[optional]] photo_width : int = INT_MIN	//! Optional. Photo width
    [[optional]] photo_height : int = INT_MIN	//! Optional. Photo height
    [[optional]] need_name : bool = false	//! Optional. Pass True if you require the user's full name to complete the order
    [[optional]] need_phone_number : bool = false	//! Optional. Pass True if you require the user's phone number to complete the order
    [[optional]] need_email : bool = false	//! Optional. Pass True if you require the user's email address to complete the order
    [[optional]] need_shipping_address : bool = false	//! Optional. Pass True if you require the user's shipping address to complete the order
    [[optional]] send_phone_number_to_provider : bool = false	//! Optional. Pass True if the user's phone number should be sent to provider
    [[optional]] send_email_to_provider : bool = false	//! Optional. Pass True if the user's email address should be sent to provider
    [[optional]] is_flexible : bool = false	//! Optional. Pass True if the final price depends on the shipping method

struct choseninlineresult
    //!  Represents a result of an inline query that was chosen by the user and sent to their chat partner.
    result_id : string	//! The unique identifier for the result that was chosen
    from : user	//! The user that chose the result
    [[optional]] location : location?	//! Optional. Sender location, only for bots that require user location
    [[optional]] inline_message_id : string = ""	//! Optional. Identifier of the sent inline message. Available only if there is an inline keyboard attached to the message. Will be also received in callback queries and can be used to edit the message.
    query : string	//! The query that was used to obtain the result

struct answerwebappquery
    //!  Use this method to set the result of an interaction with a Web App and send a corresponding message on behalf of the user to the chat from which the query originated. On success, a SentWebAppMessage object is returned.
    web_app_query_id : string	//! Required. Unique identifier for the query to be answered
    result : inlinequeryresult	//! Required. A JSON-serialized object describing the message to be sent

struct sentwebappmessage
    //!  Describes an inline message sent by a Web App on behalf of a user.
    [[optional]] inline_message_id : string = ""	//! Optional. Identifier of the sent inline message. Available only if there is an inline keyboard attached to the message.

struct sendinvoice
    //!  Use this method to send invoices. On success, the sent Message is returned.
    chat_id : string	//! Required. Unique identifier for the target chat or username of the target channel (in the format @channelusername)
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    title : string	//! Required. Product name, 1-32 characters
    description : string	//! Required. Product description, 1-255 characters
    payload : string	//! Required. Bot-defined invoice payload, 1-128 bytes. This will not be displayed to the user, use for your internal processes.
    provider_token : string	//! Required. Payment provider token, obtained via @BotFather
    currency : string	//! Required. Three-letter ISO 4217 currency code, see more on currencies
    prices : array<labeledprice>	//! Required. Price breakdown, a JSON-serialized list of components (e.g. product price, tax, discount, delivery cost, delivery tax, bonus, etc.)
    [[optional]] max_tip_amount : int = INT_MIN	//! Optional. The maximum accepted amount for tips in the smallest units of the currency (integer, not float/double). For example, for a maximum tip of US$ 1.45 pass max_tip_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies). Defaults to 0
    [[optional]] suggested_tip_amounts : array<int>	//! Optional. A JSON-serialized array of suggested amounts of tips in the smallest units of the currency (integer, not float/double). At most 4 suggested tip amounts can be specified. The suggested tip amounts must be positive, passed in a strictly increased order and must not exceed max_tip_amount.
    [[optional]] start_parameter : string = ""	//! Optional. Unique deep-linking parameter. If left empty, forwarded copies of the sent message will have a Pay button, allowing multiple users to pay directly from the forwarded message, using the same invoice. If non-empty, forwarded copies of the sent message will have a URL button with a deep link to the bot (instead of a Pay button), with the value used as the start parameter
    [[optional]] provider_data : string = ""	//! Optional. JSON-serialized data about the invoice, which will be shared with the payment provider. A detailed description of required fields should be provided by the payment provider.
    [[optional]] photo_url : string = ""	//! Optional. URL of the product photo for the invoice. Can be a photo of the goods or a marketing image for a service. People like it better when they see what they are paying for.
    [[optional]] photo_size : int = INT_MIN	//! Optional. Photo size in bytes
    [[optional]] photo_width : int = INT_MIN	//! Optional. Photo width
    [[optional]] photo_height : int = INT_MIN	//! Optional. Photo height
    [[optional]] need_name : bool = false	//! Optional. Pass True if you require the user's full name to complete the order
    [[optional]] need_phone_number : bool = false	//! Optional. Pass True if you require the user's phone number to complete the order
    [[optional]] need_email : bool = false	//! Optional. Pass True if you require the user's email address to complete the order
    [[optional]] need_shipping_address : bool = false	//! Optional. Pass True if you require the user's shipping address to complete the order
    [[optional]] send_phone_number_to_provider : bool = false	//! Optional. Pass True if the user's phone number should be sent to provider
    [[optional]] send_email_to_provider : bool = false	//! Optional. Pass True if the user's email address should be sent to provider
    [[optional]] is_flexible : bool = false	//! Optional. Pass True if the final price depends on the shipping method
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard. If empty, one 'Pay total price' button will be shown. If not empty, the first button must be a Pay button.

struct createinvoicelink
    //!  Use this method to create a link for an invoice. Returns the created invoice link as String on success.
    title : string	//! Required. Product name, 1-32 characters
    description : string	//! Required. Product description, 1-255 characters
    payload : string	//! Required. Bot-defined invoice payload, 1-128 bytes. This will not be displayed to the user, use for your internal processes.
    provider_token : string	//! Required. Payment provider token, obtained via BotFather
    currency : string	//! Required. Three-letter ISO 4217 currency code, see more on currencies
    prices : array<labeledprice>	//! Required. Price breakdown, a JSON-serialized list of components (e.g. product price, tax, discount, delivery cost, delivery tax, bonus, etc.)
    [[optional]] max_tip_amount : int = INT_MIN	//! Optional. The maximum accepted amount for tips in the smallest units of the currency (integer, not float/double). For example, for a maximum tip of US$ 1.45 pass max_tip_amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies). Defaults to 0
    [[optional]] suggested_tip_amounts : array<int>	//! Optional. A JSON-serialized array of suggested amounts of tips in the smallest units of the currency (integer, not float/double). At most 4 suggested tip amounts can be specified. The suggested tip amounts must be positive, passed in a strictly increased order and must not exceed max_tip_amount.
    [[optional]] provider_data : string = ""	//! Optional. JSON-serialized data about the invoice, which will be shared with the payment provider. A detailed description of required fields should be provided by the payment provider.
    [[optional]] photo_url : string = ""	//! Optional. URL of the product photo for the invoice. Can be a photo of the goods or a marketing image for a service.
    [[optional]] photo_size : int = INT_MIN	//! Optional. Photo size in bytes
    [[optional]] photo_width : int = INT_MIN	//! Optional. Photo width
    [[optional]] photo_height : int = INT_MIN	//! Optional. Photo height
    [[optional]] need_name : bool = false	//! Optional. Pass True if you require the user's full name to complete the order
    [[optional]] need_phone_number : bool = false	//! Optional. Pass True if you require the user's phone number to complete the order
    [[optional]] need_email : bool = false	//! Optional. Pass True if you require the user's email address to complete the order
    [[optional]] need_shipping_address : bool = false	//! Optional. Pass True if you require the user's shipping address to complete the order
    [[optional]] send_phone_number_to_provider : bool = false	//! Optional. Pass True if the user's phone number should be sent to the provider
    [[optional]] send_email_to_provider : bool = false	//! Optional. Pass True if the user's email address should be sent to the provider
    [[optional]] is_flexible : bool = false	//! Optional. Pass True if the final price depends on the shipping method

struct answershippingquery
    //!  If you sent an invoice requesting a shipping address and the parameter is_flexible was specified, the Bot API will send an Update with a shipping_query field to the bot. Use this method to reply to shipping queries. On success, True is returned.
    shipping_query_id : string	//! Required. Unique identifier for the query to be answered
    ok : bool	//! Required. Pass True if delivery to the specified address is possible and False if there are any problems (for example, if delivery to the specified address is not possible)
    [[optional]] shipping_options : array<shippingoption>	//! Optional. Required if ok is True. A JSON-serialized array of available shipping options.
    [[optional]] error_message : string = ""	//! Optional. Required if ok is False. Error message in human readable form that explains why it is impossible to complete the order (e.g. "Sorry, delivery to your desired address is unavailable'). Telegram will display this message to the user.

struct answerprecheckoutquery
    //!  Once the user has confirmed their payment and shipping details, the Bot API sends the final confirmation in the form of an Update with the field pre_checkout_query. Use this method to respond to such pre-checkout queries. On success, True is returned. Note: The Bot API must receive an answer within 10 seconds after the pre-checkout query was sent.
    pre_checkout_query_id : string	//! Required. Unique identifier for the query to be answered
    ok : bool	//! Required. Specify True if everything is alright (goods are available, etc.) and the bot is ready to proceed with the order. Use False if there are any problems.
    [[optional]] error_message : string = ""	//! Optional. Required if ok is False. Error message in human readable form that explains the reason for failure to proceed with the checkout (e.g. "Sorry, somebody just bought the last of our amazing black T-shirts while you were busy filling out your payment details. Please choose a different color or garment!"). Telegram will display this message to the user.

struct labeledprice
    //!  This object represents a portion of the price for goods or services.
    _label : string	//! Portion label
    amount : int	//! Price of the product in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).

struct invoice
    //!  This object contains basic information about an invoice.
    title : string	//! Product name
    description : string	//! Product description
    start_parameter : string	//! Unique bot deep-linking parameter that can be used to generate this invoice
    currency : string	//! Three-letter ISO 4217 currency code
    total_amount : int	//! Total price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).

struct shippingaddress
    //!  This object represents a shipping address.
    country_code : string	//! Two-letter ISO 3166-1 alpha-2 country code
    state : string	//! State, if applicable
    city : string	//! City
    street_line1 : string	//! First line for the address
    street_line2 : string	//! Second line for the address
    post_code : string	//! Address post code

struct orderinfo
    //!  This object represents information about an order.
    [[optional]] name : string = ""	//! Optional. User name
    [[optional]] phone_number : string = ""	//! Optional. User's phone number
    [[optional]] email : string = ""	//! Optional. User email
    [[optional]] shipping_address : shippingaddress?	//! Optional. User shipping address

struct shippingoption
    //!  This object represents one shipping option.
    id : string	//! Shipping option identifier
    title : string	//! Option title
    prices : array<labeledprice>	//! List of price portions

struct successfulpayment
    //!  This object contains basic information about a successful payment.
    currency : string	//! Three-letter ISO 4217 currency code
    total_amount : int	//! Total price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).
    invoice_payload : string	//! Bot specified invoice payload
    [[optional]] shipping_option_id : string = ""	//! Optional. Identifier of the shipping option chosen by the user
    [[optional]] order_info : orderinfo?	//! Optional. Order information provided by the user
    telegram_payment_charge_id : string	//! Telegram payment identifier
    provider_payment_charge_id : string	//! Provider payment identifier

struct shippingquery
    //!  This object contains information about an incoming shipping query.
    id : string	//! Unique query identifier
    from : user	//! User who sent the query
    invoice_payload : string	//! Bot specified invoice payload
    shipping_address : shippingaddress	//! User specified shipping address

struct precheckoutquery
    //!  This object contains information about an incoming pre-checkout query.
    id : string	//! Unique query identifier
    from : user	//! User who sent the query
    currency : string	//! Three-letter ISO 4217 currency code
    total_amount : int	//! Total price in the smallest units of the currency (integer, not float/double). For example, for a price of US$ 1.45 pass amount = 145. See the exp parameter in currencies.json, it shows the number of digits past the decimal point for each currency (2 for the majority of currencies).
    invoice_payload : string	//! Bot specified invoice payload
    [[optional]] shipping_option_id : string = ""	//! Optional. Identifier of the shipping option chosen by the user
    [[optional]] order_info : orderinfo?	//! Optional. Order information provided by the user

struct passportdata
    //!  Describes Telegram Passport data shared with the bot by the user.
    data : array<encryptedpassportelement>	//! Array with information about documents and other Telegram Passport elements that was shared with the bot
    credentials : encryptedcredentials	//! Encrypted credentials required to decrypt the data

struct passportfile
    //!  This object represents a file uploaded to Telegram Passport. Currently all Telegram Passport files are in JPEG format when decrypted and don't exceed 10MB.
    file_id : string	//! Identifier for this file, which can be used to download or reuse the file
    file_unique_id : string	//! Unique identifier for this file, which is supposed to be the same over time and for different bots. Can't be used to download or reuse the file.
    file_size : int	//! File size in bytes
    file_date : int	//! Unix time when the file was uploaded

struct encryptedpassportelement
    //!  Describes documents or other Telegram Passport elements shared with the bot by the user.
    _type : string	//! Element type. One of “personal_details”, “passport”, “driver_license”, “identity_card”, “internal_passport”, “address”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”, “phone_number”, “email”.
    [[optional]] data : string = ""	//! Optional. Base64-encoded encrypted Telegram Passport element data provided by the user, available for “personal_details”, “passport”, “driver_license”, “identity_card”, “internal_passport” and “address” types. Can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] phone_number : string = ""	//! Optional. User's verified phone number, available only for “phone_number” type
    [[optional]] email : string = ""	//! Optional. User's verified email address, available only for “email” type
    [[optional]] files : array<passportfile>	//! Optional. Array of encrypted files with documents provided by the user, available for “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration” and “temporary_registration” types. Files can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] front_side : passportfile?	//! Optional. Encrypted file with the front side of the document, provided by the user. Available for “passport”, “driver_license”, “identity_card” and “internal_passport”. The file can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] reverse_side : passportfile?	//! Optional. Encrypted file with the reverse side of the document, provided by the user. Available for “driver_license” and “identity_card”. The file can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] selfie : passportfile?	//! Optional. Encrypted file with the selfie of the user holding a document, provided by the user; available for “passport”, “driver_license”, “identity_card” and “internal_passport”. The file can be decrypted and verified using the accompanying EncryptedCredentials.
    [[optional]] translation : array<passportfile>	//! Optional. Array of encrypted files with translated versions of documents provided by the user. Available if requested for “passport”, “driver_license”, “identity_card”, “internal_passport”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration” and “temporary_registration” types. Files can be decrypted and verified using the accompanying EncryptedCredentials.
    hash : string	//! Base64-encoded element hash for using in PassportElementErrorUnspecified

struct encryptedcredentials
    //!  Describes data required for decrypting and authenticating EncryptedPassportElement. See the Telegram Passport Documentation for a complete description of the data decryption and authentication processes.
    data : string	//! Base64-encoded encrypted JSON-serialized data with unique user's payload, data hashes and secrets required for EncryptedPassportElement decryption and authentication
    hash : string	//! Base64-encoded data hash for data authentication
    secret : string	//! Base64-encoded secret, encrypted with the bot's public RSA key, required for data decryption

struct setpassportdataerrors
    //!  Informs a user that some of the Telegram Passport elements they provided contains errors. The user will not be able to re-submit their Passport to you until the errors are fixed (the contents of the field for which you returned the error must change). Returns True on success. Use this if the data submitted by the user doesn't satisfy the standards your service requires for any reason. For example, if a birthday date seems invalid, a submitted document is blurry, a scan shows evidence of tampering, etc. Supply some details in the error message to make sure the user knows how to correct the issues.
    user_id : int	//! Required. User identifier
    errors : array<passportelementerror>	//! Required. A JSON-serialized array describing the errors

struct passportelementerror {} //  This object represents an error in the Telegram Passport element which was submitted that should be resolved by the user. It should be one of:

struct passportelementerrordatafield
    //!  Represents an issue in one of the data fields that was provided by the user. The error is considered resolved when the field's value changes.
    source : string	//! Error source, must be data
    _type : string	//! The section of the user's Telegram Passport which has the error, one of “personal_details”, “passport”, “driver_license”, “identity_card”, “internal_passport”, “address”
    field_name : string	//! Name of the data field which has the error
    data_hash : string	//! Base64-encoded data hash
    message : string	//! Error message

struct passportelementerrorfrontside
    //!  Represents an issue with the front side of a document. The error is considered resolved when the file with the front side of the document changes.
    source : string	//! Error source, must be front_side
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”
    file_hash : string	//! Base64-encoded hash of the file with the front side of the document
    message : string	//! Error message

struct passportelementerrorreverseside
    //!  Represents an issue with the reverse side of a document. The error is considered resolved when the file with reverse side of the document changes.
    source : string	//! Error source, must be reverse_side
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “driver_license”, “identity_card”
    file_hash : string	//! Base64-encoded hash of the file with the reverse side of the document
    message : string	//! Error message

struct passportelementerrorselfie
    //!  Represents an issue with the selfie with a document. The error is considered resolved when the file with the selfie changes.
    source : string	//! Error source, must be selfie
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”
    file_hash : string	//! Base64-encoded hash of the file with the selfie
    message : string	//! Error message

struct passportelementerrorfile
    //!  Represents an issue with a document scan. The error is considered resolved when the file with the document scan changes.
    source : string	//! Error source, must be file
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hash : string	//! Base64-encoded file hash
    message : string	//! Error message

struct passportelementerrorfiles
    //!  Represents an issue with a list of scans. The error is considered resolved when the list of files containing the scans changes.
    source : string	//! Error source, must be files
    _type : string	//! The section of the user's Telegram Passport which has the issue, one of “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hashes : array<string>	//! List of base64-encoded file hashes
    message : string	//! Error message

struct passportelementerrortranslationfile
    //!  Represents an issue with one of the files that constitute the translation of a document. The error is considered resolved when the file changes.
    source : string	//! Error source, must be translation_file
    _type : string	//! Type of element of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hash : string	//! Base64-encoded file hash
    message : string	//! Error message

struct passportelementerrortranslationfiles
    //!  Represents an issue with the translated version of a document. The error is considered resolved when a file with the document translation change.
    source : string	//! Error source, must be translation_files
    _type : string	//! Type of element of the user's Telegram Passport which has the issue, one of “passport”, “driver_license”, “identity_card”, “internal_passport”, “utility_bill”, “bank_statement”, “rental_agreement”, “passport_registration”, “temporary_registration”
    file_hashes : array<string>	//! List of base64-encoded file hashes
    message : string	//! Error message

struct passportelementerrorunspecified
    //!  Represents an issue in an unspecified place. The error is considered resolved when new data is added.
    source : string	//! Error source, must be unspecified
    _type : string	//! Type of element of the user's Telegram Passport which has the issue
    element_hash : string	//! Base64-encoded element hash
    message : string	//! Error message

struct sendgame
    //!  Use this method to send a game. On success, the sent Message is returned.
    chat_id : int	//! Required. Unique identifier for the target chat
    [[optional]] message_thread_id : int = INT_MIN	//! Optional. Unique identifier for the target message thread (topic) of the forum; for forum supergroups only
    game_short_name : string	//! Required. Short name of the game, serves as the unique identifier for the game. Set up your games via @BotFather.
    [[optional]] disable_notification : bool = false	//! Optional. Sends the message silently. Users will receive a notification with no sound.
    [[optional]] protect_content : bool = false	//! Optional. Protects the contents of the sent message from forwarding and saving
    [[optional]] reply_to_message_id : int = INT_MIN	//! Optional. If the message is a reply, ID of the original message
    [[optional]] allow_sending_without_reply : bool = false	//! Optional. Pass True if the message should be sent even if the specified replied-to message is not found
    [[optional]] reply_markup : inlinekeyboardmarkup?	//! Optional. A JSON-serialized object for an inline keyboard. If empty, one 'Play game_title' button will be shown. If not empty, the first button must launch the game.

struct game
    //!  This object represents a game. Use BotFather to create and edit games, their short names will act as unique identifiers.
    title : string	//! Title of the game
    description : string	//! Description of the game
    photo : array<photosize>	//! Photo that will be displayed in the game message in chats.
    [[optional]] text : string = ""	//! Optional. Brief description of the game or high scores included in the game message. Can be automatically edited to include current high scores for the game when the bot calls setGameScore, or manually edited using editMessageText. 0-4096 characters.
    [[optional]] text_entities : array<messageentity>	//! Optional. Special entities that appear in text, such as usernames, URLs, bot commands, etc.
    [[optional]] animation : animation?	//! Optional. Animation that will be displayed in the game message in chats. Upload via BotFather

struct callbackgame {} //  A placeholder, currently holds no information. Use BotFather to set up your game.

struct setgamescore
    //!  Use this method to set the score of the specified user in a game message. On success, if the message is not an inline message, the Message is returned, otherwise True is returned. Returns an error, if the new score is not greater than the user's current score in the chat and force is False.
    user_id : int	//! Required. User identifier
    score : int	//! Required. New score, must be non-negative
    [[optional]] force : bool = false	//! Optional. Pass True if the high score is allowed to decrease. This can be useful when fixing mistakes or banning cheaters
    [[optional]] disable_edit_message : bool = false	//! Optional. Pass True if the game message should not be automatically edited to include the current scoreboard
    [[optional]] chat_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the sent message
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message

struct getgamehighscores
    //!  Use this method to get data for high score tables. Will return the score of the specified user and several of their neighbors in a game. Returns an Array of GameHighScore objects.
    user_id : int	//! Required. Target user id
    [[optional]] chat_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Unique identifier for the target chat
    [[optional]] message_id : int = INT_MIN	//! Optional. Required if inline_message_id is not specified. Identifier of the sent message
    [[optional]] inline_message_id : string = ""	//! Optional. Required if chat_id and message_id are not specified. Identifier of the inline message

struct gamehighscore
    //!  This object represents one row of the high scores table for a game.
    position : int	//! Position in high score table for the game
    user : user	//! User
    score : int	//! Score

