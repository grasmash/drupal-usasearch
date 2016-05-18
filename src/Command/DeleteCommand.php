<?php

namespace Drupal\usasearch\Command;

use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Drupal\Console\Command\Command;
use Drupal\Console\Style\DrupalStyle;
use Drupal\node\Entity\Node;

/**
 * Class DeleteCommand.
 *
 * @package Drupal\usasearch
 */
class DeleteCommand extends Command {

  /**
   * {@inheritdoc}
   */
  protected function configure() {
    $this
      ->setName('usasearch:delete')
      ->setDescription($this->trans('command.usasearch.delete.description'))
      ->addArgument('id', InputArgument::OPTIONAL, $this->trans('command.usasearch.delete.arguments.id'))
      ->addOption('all', NULL, InputOption::VALUE_NONE, $this->trans('command.usasearch.delete.options.all'));
  }

  /**
   * {@inheritdoc}
   */
  protected function execute(InputInterface $input, OutputInterface $output) {

    $io = new DrupalStyle($input, $output);
    $id = $input->getArgument('id');
    $all = $input->getOption('all');
    $yes = $input->hasOption('yes') ? $input->getOption('yes') : FALSE;
    $api = \Drupal::service('usasearch.api');

    if ($all) {
      if (!$yes) {
        // Get confirmation before performing bulk delete.
        $confirmation = $io->confirm(
          $this->trans('command.usasearch.delete.questions.confirm'),
          TRUE
        );
        if (!$confirmation) {
          $io->warning($this->trans('command.usasearch.delete.messages.canceled'));
          return;
        }
      }
      $ids = array_keys(Node::loadMultiple());
      // Remove each document from the i14y API.
      foreach ($ids as $id) {
        $api->request('delete', 'api/v1/documents/' . $id);
      }
      $msg = sprintf(
        'Deleted %s nodes.',
        count($ids)
      );
      $io->success($msg);
    }
    elseif ($id && is_numeric($id)) {
      $api->request('delete', 'api/v1/documents/' . $id);
      $msg = sprintf(
        'Deleted node: %s',
        $id
      );
      $io->success($msg);
    }
    else {
      // No actionable arguments/options where provided.
      $io->warning($this->trans('command.usasearch.delete.messages.missing'));
    }

  }

}
