-- Queries used to add the correct tables and relationships to the bank_system database.

CREATE TABLE `account_types` (
  `type_id` int(3) NOT NULL,
  `type` varchar(50) NOT NULL
);



CREATE TABLE `active_accounts` (
  `account_number` int(3) NOT NULL,
  `user_id` int(11) NOT NULL,
  `type_id` int(11) NOT NULL,
  `balance` double NOT NULL
);


CREATE TABLE `users` (
  `user_id` int(11) NOT NULL,
  `email` varchar(100) NOT NULL,
  `password` varchar(32) NOT NULL,
  `name` varchar(50) NOT NULL,
  `address` varchar(100) NOT NULL,
  `phone` varchar(14) NOT NULL
);


ALTER TABLE `account_types`
  ADD PRIMARY KEY (`type_id`);


ALTER TABLE `active_accounts`
  ADD PRIMARY KEY (`account_number`);


ALTER TABLE `users`
  ADD PRIMARY KEY (`user_id`);


ALTER TABLE `account_types`
  MODIFY `type_id` int(3) NOT NULL AUTO_INCREMENT;


ALTER TABLE `active_accounts`
  MODIFY `account_number` int(3) NOT NULL AUTO_INCREMENT;


ALTER TABLE `users`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT;


ALTER TABLE `active_accounts`
  ADD FOREIGN KEY (`user_id`) REFERENCES `users`(`user_id`);


ALTER TABLE `active_accounts`
  ADD FOREIGN KEY (`type_id`) REFERENCES `account_types`(`type_id`);
